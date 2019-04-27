auto MCD::read(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  if(address >= 0x000000 && address <= 0x07ffff) {
    return pram[address >> 1];
  }

  if(address >= 0x080000 && address <= 0x0bffff && io.wramMode == 0) {
    if(io.wramSwitch == 0) return data;
    return wram[address >> 1];
  }

  if(address >= 0x080000 && address <= 0x0dffff && io.wramMode == 1) {
    address = io.wramSelect << 17 | (uint17)address;
    return wram[address >> 1];
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    if(!lower) return data;
    return bram.read(address >> 1);
  }

  if(address >= 0xff0000 && address <= 0xff7fff) {
    if(!lower) return data;
    return pcm.read(address >> 1, data);
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    return readIO(upper, lower, address, data);
  }

//print("* ur", hex(address), "\n");
  return data;
}

auto MCD::write(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x07ffff) {
    if(upper) pram[address >> 1].byte(1) = data.byte(1);
    if(lower) pram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x080000 && address <= 0x0bffff && io.wramMode == 0) {
    if(io.wramSwitch == 0) return;
    if(upper) wram[address >> 1].byte(1) = data.byte(1);
    if(lower) wram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x080000 && address <= 0x0dffff && io.wramMode == 1) {
    address = io.wramSelect << 17 | (uint17)address;
    if(upper) wram[address >> 1].byte(1) = data.byte(1);
    if(lower) wram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    if(!lower) return;
    return bram.write(address >> 1, data);
  }

  if(address >= 0xff0000 && address <= 0xff7fff) {
    if(!lower) return;
    return pcm.write(address >> 1, data);
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    return writeIO(upper, lower, address, data);
  }

//print("* uw", hex(address), "=", hex(data), "\n");
}
