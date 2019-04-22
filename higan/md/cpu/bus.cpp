auto CPU::readByte(uint24 address) -> uint16 {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(!io.romEnable) return tmss[address & 0x7ff];
    if(cartridge.node) return cartridge.read(address & ~1).byte(!address.bit(0));
    if(expansion.node) return expansion.read(address);
    return 0x0000;
  }
  if(address >= 0x400000 && address <= 0x7fffff) {
    if(!cartridge.node) return 0x0000;
    if(!expansion.node) return 0x0000;
    return expansion.read(address);
  }
  if(address >= 0xa00000 && address <= 0xa0ffff) return apu.granted() ? apu.read(address) : (uint8)0x00;
  if(address >= 0xa10000 && address <= 0xa10fff) return readIO(address & ~0xff00);
  if(address >= 0xa11000 && address <= 0xa11fff) return readIO(address & ~0x00ff);
  if(address >= 0xa12000 && address <= 0xa12fff) return expansion.readIO(address);
  if(address >= 0xa13000 && address <= 0xa13fff) return cartridge.readIO(address);
  if(address >= 0xc00000 && address <= 0xdfffff) return vdp.read(address & ~1).byte(!address.bit(0));
  if(address >= 0xe00000 && address <= 0xffffff) return ram.read(address);
  return 0x0000;
}

auto CPU::readWord(uint24 address) -> uint16 {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(!io.romEnable) {
      uint16 data = tmss[address ^ 0] << 8;
      return data | tmss[address ^ 1] << 0;
    }
    if(cartridge.node) return cartridge.read(address);
    if(expansion.node) {
      uint16 data = expansion.read(address ^ 0) << 8;
      return data | expansion.read(address ^ 1) << 0;
    }
    return 0x0000;
  }
  if(address >= 0x400000 && address <= 0x7fffff) {
    if(!cartridge.node) return 0x0000;
    if(!expansion.node) return 0x0000;
    uint16 data = expansion.read(address ^ 0) << 8;
    return data | expansion.read(address ^ 1) << 0;
  }
  if(address >= 0xa00000 && address <= 0xa0ffff) return apu.granted() ? apu.read(address) : (uint8)0x00;
  if(address >= 0xa10000 && address <= 0xa10fff) return readIO(address & ~0xff00) << 0;
  if(address >= 0xa11000 && address <= 0xa11fff) return readIO(address & ~0x00ff) << 8;
  if(address >= 0xa12000 && address <= 0xa12fff) {
    uint16 data = expansion.readIO(address ^ 0) << 8;
    return data | expansion.readIO(address ^ 1) << 0;
  }
  if(address >= 0xa13000 && address <= 0xa13fff) {
    uint16 data = cartridge.readIO(address ^ 0) << 8;
    return data | cartridge.readIO(address ^ 1) << 0;
  }
  if(address >= 0xc00000 && address <= 0xdfffff) return vdp.read(address);
  if(address >= 0xe00000 && address <= 0xffffff) {
    uint16 data = ram.read(address ^ 0) << 8;
    return data | ram.read(address ^ 1) << 0;
  }
  return 0x0000;
}

auto CPU::writeByte(uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(cartridge.node) return cartridge.write(address & ~1, data << 8 | data << 0);
    if(expansion.node) return expansion.write(address, data);
    return;
  }
  if(address >= 0x400000 && address <= 0x7fffff) {
    if(!cartridge.node) return;
    if(!expansion.node) return;
    return expansion.write(address, data);
  }
  if(address >= 0xa00000 && address <= 0xa0ffff) return apu.granted() ? apu.write(address, data) : (void)0;
  if(address >= 0xa10000 && address <= 0xa10fff) return writeIO(address & ~0xff00, data);
  if(address >= 0xa11000 && address <= 0xa11fff) return writeIO(address & ~0x00ff, data);
  if(address >= 0xa12000 && address <= 0xa12fff) return expansion.writeIO(address, data);
  if(address >= 0xa13000 && address <= 0xa13fff) return cartridge.writeIO(address, data);
  if(address >= 0xa14000 && address <= 0xa140ff) return writeIO(address & ~0xff00, data);
  if(address >= 0xa14100 && address <= 0xa141ff) return writeIO(address & ~0x00ff, data);
  if(address >= 0xc00000 && address <= 0xc0000f) return vdp.write(address & ~1, data << 8 | data << 0);
  if(address >= 0xc00010 && address <= 0xc00017) return psg.write(data);
  if(address >= 0xe00000 && address <= 0xffffff) return ram.write(address, data);
}

auto CPU::writeWord(uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(cartridge.node) return cartridge.write(address, data);
    if(expansion.node) {
      expansion.write(address ^ 0, data >> 8);
      expansion.write(address ^ 1, data >> 0);
      return;
    }
    return;
  }
  if(address >= 0x400000 && address <= 0x7fffff) {
    if(!cartridge.node) return;
    if(!expansion.node) return;
    expansion.write(address ^ 0, data >> 8);
    expansion.write(address ^ 1, data >> 0);
    return;
  }
  if(address >= 0xa00000 && address <= 0xa0ffff) return apu.granted() ? apu.write(address, data) : (void)0;
  if(address >= 0xa10000 && address <= 0xa10fff) return writeIO(address & ~0xff00, data >> 0);
  if(address >= 0xa11000 && address <= 0xa11fff) return writeIO(address & ~0x00ff, data >> 8);
  if(address >= 0xa12000 && address <= 0xa12fff) {
    expansion.writeIO(address ^ 0, data >> 8);
    expansion.writeIO(address ^ 1, data >> 0);
    return;
  }
  if(address >= 0xa13000 && address <= 0xa13fff) {
    cartridge.writeIO(address ^ 0, data >> 8);
    cartridge.writeIO(address ^ 1, data >> 0);
    return;
  }
  if(address >= 0xa14000 && address <= 0xa140ff) return writeIO(address & ~0xff00, data >> 0);
  if(address >= 0xa14100 && address <= 0xa141ff) return writeIO(address & ~0x00ff, data >> 8);
  if(address >= 0xc00000 && address <= 0xc0000f) return vdp.write(address, data);
  if(address >= 0xc00010 && address <= 0xc00017) return psg.write(data);
  if(address >= 0xe00000 && address <= 0xffffff) {
    ram.write(address ^ 0, data >> 8);
    ram.write(address ^ 1, data >> 0);
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
