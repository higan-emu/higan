auto CDPU::readByte(uint24 address) -> uint16 {
  if(address >= 0x000000 && address <= 0x07ffff) {
    return pram[address];
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    return wram[address];
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    return bram[address >> 1];
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    return readIO(address);
  }

//print("* rb", hex(address), "\n");
  return 0x0000;
}

auto CDPU::readWord(uint24 address) -> uint16 {
  if(address >= 0x000000 && address <= 0x07ffff) {
    uint16 data = pram[address ^ 0] << 8;
    return data | pram[address ^ 1] << 0;
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    uint16 data = wram[address ^ 0] << 8;
    return data | wram[address ^ 1] << 0;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    return bram[address >> 1];
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    uint16 data = readIO(address ^ 0) << 8;
    return data | readIO(address ^ 1) << 0;
  }

//print("* rw", hex(address), "\n");
  return 0x0000;
}

auto CDPU::writeByte(uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x07ffff) {
    pram[address] = data;
    return;
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    wram[address] = data;
    return;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    bram[address >> 1] = data;
    return;
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    writeIO(address, data);
    return;
  }

//print("* wb", hex(address), "\n");
}

auto CDPU::writeWord(uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x07ffff) {
    pram[address ^ 0] = data >> 8;
    pram[address ^ 1] = data >> 0;
    return;
  }

  if(address >= 0x080000 && address <= 0x0bffff) {
    wram[address ^ 0] = data >> 8;
    wram[address ^ 1] = data >> 0;
    return;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    bram[address >> 1] = data;
    return;
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    writeIO(address ^ 0, data >> 8);
    writeIO(address ^ 1, data >> 0);
    return;
  }

//print("* ww", hex(address), "\n");
}
