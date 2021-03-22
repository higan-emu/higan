auto MCD::read(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  address.bit(20,23) = 0;  //mirroring

  if(address >= 0x000000 && address <= 0x07ffff) {
    return pram[address >> 1];
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    if(io.wramMode == 0) {
    //if(io.wramSwitch == 0) return data;
      address = (uint18)address;
      return wram[address >> 1];
    }
    return data;
  }

  if(address >= 0x0c0000 && address <= 0x0dffff) {
    if(io.wramMode == 1) {
      if(io.wramSelect == 0)
        address = (uint17)address;
      else
        address = (uint17)address + 0x020000;
      return wram[address >> 1];
    }
    return data;
  }

  if(address >= 0x0e0000 && address <= 0x0effff) {
    if(!lower) return data;
    return bram.read(address >> 1);
  }

  if(address >= 0x0f0000 && address <= 0x0f7fff) {
    if(!lower) return data;
    return pcm.read(address >> 1, data);
  }

  if(address >= 0x0f8000 && address <= 0x0fffff) {
    return readIO(upper, lower, address, data);
  }

  return data;  //unreachable
}

auto MCD::write(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  address.bit(20,23) = 0;  //mirroring

  if(address >= 0x000000 && address <= 0x07ffff) {
    if(upper) pram[address >> 1].byte(1) = data.byte(1);
    if(lower) pram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    if(io.wramMode == 0) {
    //if(io.wramSwitch == 0) return;
      address = (uint18)address;
      if(upper) wram[address >> 1].byte(1) = data.byte(1);
      if(lower) wram[address >> 1].byte(0) = data.byte(0);
    }
    return;
  }

  if(address >= 0x0c0000 && address <= 0x0dffff) {
    if(io.wramMode == 1) {
      if(io.wramSelect == 0)
        address = (uint17)address;
      else
        address = (uint17)address + 0x020000;
      if(upper) wram[address >> 1].byte(1) = data.byte(1);
      if(lower) wram[address >> 1].byte(0) = data.byte(0);
    }
    return;
  }

  if(address >= 0x0e0000 && address <= 0x0effff) {
    if(!lower) return;
    return bram.write(address >> 1, data);
  }

  if(address >= 0x0f0000 && address <= 0x0f7fff) {
    if(!lower) return;
    return pcm.write(address >> 1, data);
  }

  if(address >= 0x0f8000 && address <= 0x0fffff) {
    return writeIO(upper, lower, address, data);
  }

  return;  //unreachable
}
