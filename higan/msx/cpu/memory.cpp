auto CPU::read(uint16 address) -> uint8 {
  if(address == 0xffff) return readSecondarySlot();

  uint2 page = address.bits(14,15);
  uint2 primary = slot[page].primary;
  uint2 secondary = slot[primary].secondary[page];

  if(primary == 0) {
    return system.bios.read(address);
  }

  if(primary == 1) {
    return cartridge.read(address);
  }

  if(primary == 2) {
    return expansion.read(address);
  }

  if(primary == 3) {
    if(secondary == 0) {
      if(Model::MSX()) return ram.read(address);
      uint22 logical = slot[page].memory << 14 | (uint14)address;
      return ram.read(logical);
    }
    if(secondary == 1 && system.sub) {
      return system.sub.read(address);
    }
  }

  return 0xff;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  if(address == 0xffff) return writeSecondarySlot(data);

  uint2 page = address.bits(14,15);
  uint2 primary = slot[page].primary;
  uint2 secondary = slot[primary].secondary[page];

  if(primary == 0) {
    return;
  }

  if(primary == 1) {
    return cartridge.write(address, data);
  }

  if(primary == 2) {
    return expansion.write(address, data);
  }

  if(primary == 3) {
    if(Model::MSX()) return ram.write(address, data);
    uint22 logical = slot[page].memory << 14 | (uint14)address;
    return ram.write(logical, data);
  }
}

//

auto CPU::in(uint16 address) -> uint8 {
  switch((uint8)address) {
  case 0x98: return vdp.data();
  case 0x99: return vdp.status();
  case 0xa2: return psg.read();
  case 0xa8: return readPrimarySlot();
  case 0xa9: return keyboard.read();
  case 0xfc: case 0xfd: case 0xfe: case 0xff:
    if(Model::MSX()) return 0xff;
    //note: reading these ports is said to be unreliable;
    //but since it's not specified how so, that is not emulated here
    return slot[(uint2)address].memory;
  }
  return 0xff;
}

auto CPU::out(uint16 address, uint8 data) -> void {
  switch((uint8)address) {
  case 0x98: return vdp.data(data);
  case 0x99: return vdp.control(data);
  case 0xa0: return psg.select(data);
  case 0xa1: return psg.write(data);
  case 0xa8: return writePrimarySlot(data);
  case 0xaa: return keyboard.write(data.bits(0,3));
  case 0xfc: case 0xfd: case 0xfe: case 0xff:
    slot[(uint2)address].memory = data;
    return;
  }
}

//

auto CPU::readPrimarySlot() -> uint8 {
  uint8 data;
  data.bits(0,1) = slot[0].primary;
  data.bits(2,3) = slot[1].primary;
  data.bits(4,5) = slot[2].primary;
  data.bits(6,7) = slot[3].primary;
  return data;
}

auto CPU::writePrimarySlot(uint8 data) -> void {
  slot[0].primary = data.bits(0,1);
  slot[1].primary = data.bits(2,3);
  slot[2].primary = data.bits(4,5);
  slot[3].primary = data.bits(6,7);
}

//

auto CPU::readSecondarySlot() -> uint8 {
  auto primary = slot[3].primary;
  uint8 data;
  data.bits(0,1) = slot[primary].secondary[0];
  data.bits(2,3) = slot[primary].secondary[1];
  data.bits(4,5) = slot[primary].secondary[2];
  data.bits(6,7) = slot[primary].secondary[3];
  return ~data;
}

auto CPU::writeSecondarySlot(uint8 data) -> void {
  auto primary = slot[3].primary;
  slot[primary].secondary[0] = data.bits(0,1);
  slot[primary].secondary[1] = data.bits(2,3);
  slot[primary].secondary[2] = data.bits(4,5);
  slot[primary].secondary[3] = data.bits(6,7);
}
