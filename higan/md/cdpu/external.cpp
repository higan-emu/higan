auto CDPU::external_read(uint22 address) -> uint8 {
  uint8 data;
  if(address >= 0x000000 && address <= 0x01ffff) {
    data = bios[(uint17)address];
  }
  if(address >= 0x020000 && address <= 0x03ffff) {
    data = pram[io.pramBank << 17 | (uint17)address];
  }
  if(address >= 0x200000 && address <= 0x23ffff) {
    data = wram[(uint18)address];
  }
  return data;
}

auto CDPU::external_write(uint22 address, uint8 data) -> void {
  if(address >= 0x020000 && address <= 0x03ffff) {
    pram[io.pramBank << 17 | (uint17)address] = data;
  }
  if(address >= 0x200000 && address <= 0x23ffff) {
    wram[(uint18)address] = data;
  }
}

//

auto CDPU::external_readIO(uint24 address) -> uint8 {
  uint8 data;

  switch(address) {
  case 0xa12000:
    data.bit(0) = !irq.external.pending;
    break;

  case 0xa12001:
    data.bit(0) = io.run;
    data.bit(1) = io.request;
    break;

  case 0xa12002:
    data = io.pramProtect;
    break;

  case 0xa12003:
    data.bit(0) = 1;
    data.bit(1) = 1;
    data.bit(2) = io.wramMode;
    data.bits(6,7) = io.pramBank;
    break;

  case 0xa12006:
    data = bios.read(0x72);
    break;

  case 0xa12007:
    data = bios.read(0x73);
    break;

  case 0xa1200e:
    data = io.cfm;
    break;

  case 0xa1200c:
    data.bits(0,3) = io.stopwatch.bits(8,11);
    break;

  case 0xa1200d:
    data.bits(0,7) = io.stopwatch.bits(0,7);
    break;

  case 0xa1200f:
    data = io.cfs;
    break;
  }

  if(address >= 0xa12010 && address <= 0xa1201f) data = io.command[address - 0xa12010];
  if(address >= 0xa12020 && address <= 0xa1202f) data = io.status [address - 0xa12020];

//print("! r", hex(address, 6), " = ", hex(data, 2), "\n");
  return data;
}

auto CDPU::external_writeIO(uint24 address, uint8 data) -> void {
  switch(address) {
  case 0xa12000:
    irq.external.pending |= irq.external.enable;
    irq.synchronize();
    break;

  case 0xa12001:
    if(io.run && !data.bit(0)) power(true);
    io.run     = data.bit(0);
    io.request = data.bit(1);
    io.halt    = !io.run || io.request;
    break;

  case 0xa12002:
    io.pramProtect = data;
    break;

  case 0xa12003:
    io.pramBank = data.bits(6,7);
    break;

  case 0xa12006:
    bios.program(0x72, data);
    break;

  case 0xa12007:
    bios.program(0x73, data);
    break;

  case 0xa1200e:
    io.cfm = data;
    break;
  }

  if(address >= 0xa12010 && address <= 0xa1201f) io.command[address - 0xa12010] = data;

//print("! w", hex(address, 6), " = ", hex(data, 2), "\n");
}
