auto CPU::read(uint1 size, uint24 address, uint16 data) -> uint16 {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(!io.romEnable) {
      data.byte(1) = tmss[address ^ zero];
      data.byte(0) = tmss[address ^ size];
      return data;
    }
    if(cartridge.node) return data = cartridge.read(size, address, data);
    if(expansion.node) return data = expansion.read(size, address, data);
    return data;
  }

  if(address >= 0x400000 && address <= 0x7fffff) {
    if(expansion.node) return expansion.read(size, address, data);
  }

  if(address >= 0xa00000 && address <= 0xa0ffff) {
    if(!apu.granted()) return data = 0x0000;
    return data = apu.read(address);
  }

  if(address >= 0xa10000 && address <= 0xa10fff) {
    data.byte(zero) = readIO(address & ~0xff00);
    return data;
  }

  if(address >= 0xa11000 && address <= 0xa11fff) {
    data.byte(size) = readIO(address & ~0x00ff);
    return data;
  }

  if(address >= 0xa12000 && address <= 0xa12fff) {
    return data = expansion.readIO(size, address, data);
  }

  if(address >= 0xa13000 && address <= 0xa13fff) {
    return data = cartridge.readIO(size, address, data);
  }

  if(address >= 0xc00000 && address <= 0xc0000f) {
    if(size == Byte) return data = vdp.read(address & ~1, data).byte(!address.bit(0));
    if(size == Word) return data = vdp.read(address & ~0, data);
    return data;
  }

  if(address >= 0xe00000 && address <= 0xffffff) {
    data.byte(1) = ram[address ^ zero];
    data.byte(0) = ram[address ^ size];
    return data;
  }

  return data;
}

auto CPU::write(uint1 size, uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(cartridge.node) return cartridge.write(size, address, data);
    if(expansion.node) return expansion.write(size, address, data);
    return;
  }

  if(address >= 0x400000 && address <= 0x7fffff) {
    if(expansion.node) return expansion.write(size, address, data);
  }

  if(address >= 0xa00000 && address <= 0xa0ffff) {
    if(!apu.granted()) return;
    return apu.write(address, data);
  }

  if(address >= 0xa10000 && address <= 0xa10fff) {
    return writeIO(address & ~0xff00, data.byte(zero));
  }

  if(address >= 0xa11000 && address <= 0xa11fff) {
    return writeIO(address & ~0x00ff, data.byte(size));
  }

  if(address >= 0xa12000 && address <= 0xa12fff) {
    return expansion.writeIO(size, address, data);
  }

  if(address >= 0xa13000 && address <= 0xa13fff) {
    return cartridge.writeIO(size, address, data);
  }

  if(address >= 0xa14000 && address <= 0xa140ff) {
    return writeIO(address & ~0xff00, data.byte(zero));
  }

  if(address >= 0xa14100 && address <= 0xa141ff) {
    return writeIO(address & ~0x00ff, data.byte(size));
  }

  if(address >= 0xc00000 && address <= 0xc0000f) {
    if(size == Byte) return vdp.write(address & ~1, data << 8 | data << 0);
    if(size == Word) return vdp.write(address & ~0, data);
    return;
  }

  if(address >= 0xc00010 && address <= 0xc00017) {
    return psg.write(data);
  }

  if(address >= 0xe00000 && address <= 0xffffff) {
    ram[address ^ zero] = data.byte(1);
    ram[address ^ size] = data.byte(0);
    return;
  }
}

auto CPU::readIO(uint24 address) -> uint16 {
  uint16 data;

  switch(address & ~1) {
  case 0xa10000:
    data.bit(0) =  io.version;       //0 = Model 1; 1 = Model 2+
    data.bit(5) = !MegaCD();         //0 = expansion unit connected; 1 = no expansion unit connected
    data.bit(6) =  Region::PAL();    //0 = NTSC; 1 = PAL
    data.bit(7) = !Region::NTSCJ();  //0 = domestic (Japan); 1 = export
    return data;

  case 0xa10002: return controllerPort1.readData();
  case 0xa10004: return controllerPort2.readData();
  case 0xa10006: return extensionPort.readData();

  case 0xa10008: return controllerPort1.readControl();
  case 0xa1000a: return controllerPort2.readControl();
  case 0xa1000c: return extensionPort.readControl();

  case 0xa11100: return !apu.granted();
  }

  return 0x0000;
}

auto CPU::writeIO(uint24 address, uint16 data) -> void {
  switch(address & ~1) {
  case 0xa10002: return controllerPort1.writeData(data);
  case 0xa10004: return controllerPort2.writeData(data);
  case 0xa10006: return extensionPort.writeData(data);

  case 0xa10008: return controllerPort1.writeControl(data);
  case 0xa1000a: return controllerPort2.writeControl(data);
  case 0xa1000c: return extensionPort.writeControl(data);

  case 0xa11100: return apu.request(data.bit(0));
  case 0xa11200: return apu.enable(data.bit(0));

  case 0xa14000:
    io.vdpEnable[0] = data == 0x5345;
    return;

  case 0xa14002:
    io.vdpEnable[1] = data == 0x4741;
    return;

  case 0xa14100:
    io.romEnable = data.bit(0);
    return;
  }
}
