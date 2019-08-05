auto CPU::readRAM(uint24 address, uint8 data) -> uint8 {
  return wram[address];
}

auto CPU::readAPU(uint24 address, uint8 data) -> uint8 {
  synchronize(smp);
  return smp.portRead(address.range(0,1));
}

auto CPU::readCPU(uint24 address, uint8 data) -> uint8 {
  switch((uint16)address) {
  case 0x2180:  //WMDATA
    return bus.read(0x7e0000 | io.wramAddress++, data);

  case 0x4016:  //JOYSER0
    data &= 0xfc;
    data |= controllerPort1.data();
    return data;

  case 0x4017:  //JOYSER1
    data &= 0xe0;
    data |= 0x1c;  //pins are connected to GND
    data |= controllerPort2.data();
    return data;

  case 0x4210:  //RDNMI
    data &= 0x70;
    data |= rdnmi() << 7;
    data |= (uint4)io.version;
    return data;

  case 0x4211:  //TIMEUP
    data &= 0x7f;
    data |= timeup() << 7;
    return data;

  case 0x4212:  //HVBJOY
    data &= 0x3e;
    data |= (status.autoJoypadActive) << 0;
    data |= (hcounter() <= 2 || hcounter() >= 1096) << 6;  //hblank
    data |= (vcounter() >= ppu.vdisp()) << 7;              //vblank
    return data;

  case 0x4213: return io.pio;          //RDIO

  case 0x4214: return io.rddiv >> 0;  //RDDIVL
  case 0x4215: return io.rddiv >> 8;  //RDDIVH
  case 0x4216: return io.rdmpy >> 0;  //RDMPYL
  case 0x4217: return io.rdmpy >> 8;  //RDMPYH

  case 0x4218: return io.joy1 >> 0;   //JOY1L
  case 0x4219: return io.joy1 >> 8;   //JOY1H
  case 0x421a: return io.joy2 >> 0;   //JOY2L
  case 0x421b: return io.joy2 >> 8;   //JOY2H
  case 0x421c: return io.joy3 >> 0;   //JOY3L
  case 0x421d: return io.joy3 >> 8;   //JOY3H
  case 0x421e: return io.joy4 >> 0;   //JOY4L
  case 0x421f: return io.joy4 >> 8;   //JOY4H

  }

  return data;
}

auto CPU::readDMA(uint24 address, uint8 data) -> uint8 {
  auto& channel = this->channels[address.range(4,6)];

  switch(address & 0xff8f) {

  case 0x4300:  //DMAPx
    return (
      channel.transferMode    << 0
    | channel.fixedTransfer   << 3
    | channel.reverseTransfer << 4
    | channel.unused          << 5
    | channel.indirect        << 6
    | channel.direction       << 7
    );

  case 0x4301: return channel.targetAddress;       //BBADx
  case 0x4302: return channel.sourceAddress >> 0;  //A1TxL
  case 0x4303: return channel.sourceAddress >> 8;  //A1TxH
  case 0x4304: return channel.sourceBank;          //A1Bx
  case 0x4305: return channel.transferSize >> 8;   //DASxL
  case 0x4306: return channel.transferSize >> 8;   //DASxH
  case 0x4307: return channel.indirectBank;        //DASBx
  case 0x4308: return channel.hdmaAddress >> 0;    //A2AxL
  case 0x4309: return channel.hdmaAddress >> 8;    //A2AxH
  case 0x430a: return channel.lineCounter;         //NTRLx
  case 0x430b: return channel.unknown;             //???x
  case 0x430f: return channel.unknown;             //???x ($43xb mirror)

  }

  return data;
}

auto CPU::writeRAM(uint24 address, uint8 data) -> void {
  wram[address] = data;
}

auto CPU::writeAPU(uint24 address, uint8 data) -> void {
  synchronize(smp);
  return smp.portWrite(address.range(0,1), data);
}

auto CPU::writeCPU(uint24 address, uint8 data) -> void {
  switch((uint16)address) {

  case 0x2180:  //WMDATA
    return bus.write(0x7e0000 | io.wramAddress++, data);

  case 0x2181:  //WMADDL
    io.wramAddress.range(0,7) = data.range(0,7);
    return;

  case 0x2182:  //WMADDM
    io.wramAddress.range(8,15) = data.range(0,7);
    return;

  case 0x2183:  //WMADDH
    io.wramAddress.field(16) = data.field(0);
    return;

  case 0x4016:  //JOYSER0
    //bit 0 is shared between JOYSER0 and JOYSER1:
    //strobing $4016.d0 affects both controller port latches.
    //$4017 bit 0 writes are ignored.
    controllerPort1.latch(data.field(0));
    controllerPort2.latch(data.field(0));
    return;

  case 0x4200:  //NMITIMEN
    io.autoJoypadPoll = data.field(0);
    nmitimenUpdate(data);
    return;

  case 0x4201:  //WRIO
    if(io.pio.field(7) && !data.field(7)) ppu.latchCounters();
    io.pio = data;
    return;

  case 0x4202:  //WRMPYA
    io.wrmpya = data;
    return;

  case 0x4203:  //WRMPYB
    io.rdmpy = 0;
    if(alu.mpyctr || alu.divctr) return;

    io.wrmpyb = data;
    io.rddiv = (io.wrmpyb << 8) | io.wrmpya;

    alu.mpyctr = 8;  //perform multiplication over the next eight cycles
    alu.shift = io.wrmpyb;
    return;

  case 0x4204:  //WRDIVL
    io.wrdiva.range(0, 7) = data;
    return;

  case 0x4205:  //WRDIVH
    io.wrdiva.range(8,15) = data;
    return;

  case 0x4206:  //WRDIVB
    io.rdmpy = io.wrdiva;
    if(alu.mpyctr || alu.divctr) return;

    io.wrdivb = data;

    alu.divctr = 16;  //perform division over the next sixteen cycles
    alu.shift = io.wrdivb << 16;
    return;

  case 0x4207:  //HTIMEL
    io.htime = (io.htime >> 2) - 1;
    io.htime.range(0,7) = data.range(0,7);
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4208:  //HTIMEH
    io.htime = (io.htime >> 2) - 1;
    io.htime.field(8) = data.field(0);
    io.htime = (io.htime + 1) << 2;
    return;

  case 0x4209:  //VTIMEL
    io.vtime.range(0,7) = data.range(0,7);
    return;

  case 0x420a:  //VTIMEH
    io.vtime.field(8) = data.field(0);
    return;

  case 0x420b:  //DMAEN
    for(uint n : range(8)) channels[n].dmaEnable = data.field(n);
    if(data) status.dmaPending = true;
    return;

  case 0x420c:  //HDMAEN
    for(uint n : range(8)) channels[n].hdmaEnable = data.field(n);
    return;

  case 0x420d:  //MEMSEL
    io.romSpeed = data.field(0) ? 6 : 8;
    return;

  }
}

auto CPU::writeDMA(uint24 address, uint8 data) -> void {
  auto& channel = this->channels[address.range(4,6)];

  switch(address & 0xff8f) {

  case 0x4300:  //DMAPx
    channel.transferMode    = data.range(0,2);
    channel.fixedTransfer   = data.field(3);
    channel.reverseTransfer = data.field(4);
    channel.unused          = data.field(5);
    channel.indirect        = data.field(6);
    channel.direction       = data.field(7);
    return;

  case 0x4301:  //BBADx
    channel.targetAddress = data;
    return;

  case 0x4302:  //A1TxL
    channel.sourceAddress.range(0, 7) = data;
    return;

  case 0x4303:  //A1TxH
    channel.sourceAddress.range(8,15) = data;
    return;

  case 0x4304:  //A1Bx
    channel.sourceBank = data;
    return;

  case 0x4305:  //DASxL
    channel.transferSize.range(0, 7) = data;
    return;

  case 0x4306:  //DASxH
    channel.transferSize.range(8,15) = data;
    return;

  case 0x4307:  //DASBx
    channel.indirectBank = data;
    return;

  case 0x4308:  //A2AxL
    channel.hdmaAddress.range(0, 7) = data;
    return;

  case 0x4309:  //A2AxH
    channel.hdmaAddress.range(8,15) = data;
    return;

  case 0x430a:  //NTRLx
    channel.lineCounter = data;
    return;

  case 0x430b:  //???x
    channel.unknown = data;
    return;

  case 0x430f:  //???x ($43xb mirror)
    channel.unknown = data;
    return;

  }
}
