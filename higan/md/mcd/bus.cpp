auto MCD::read(uint1 size, uint24 address, uint16 data) -> uint16 {
  if(address >= 0x000000 && address <= 0x07ffff) {
    data.byte(1) = pram[address ^ zero];
    data.byte(0) = pram[address ^ size];
    return data;
  }

  if(address >= 0x080000 && address <= 0x0bffff && io.wramMode == 0) {
  //if(io.wramSwitch == 0) return data;
    data.byte(1) = wram[address ^ zero];
    data.byte(0) = wram[address ^ size];
    return data;
  }

  if(address >= 0x080000 && address <= 0x0dffff && io.wramMode == 1) {
    address = io.wramSelect << 17 | (uint17)address;
    data.byte(1) = wram[address ^ zero];
    data.byte(0) = wram[address ^ size];
    return data;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    data = bram[address >> 1];
    return data;
  }

  if(address >= 0xff0000 && address <= 0xff7fff) {
    if(size == Byte && !address.bit(0)) return data = 0x00;
    return data = pcm.read(address >> 1, data);
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    return data = readIO(size, address, data);
  }

//print("* ur", hex(address), "\n");
  return data;
}

auto MCD::write(uint1 size, uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x07ffff) {
    pram[address ^ zero] = data.byte(1);
    pram[address ^ size] = data.byte(0);
    return;
  }

  if(address >= 0x080000 && address <= 0x0bffff && io.wramMode == 0) {
  //if(io.wramSwitch == 0) return;
    wram[address ^ zero] = data.byte(1);
    wram[address ^ size] = data.byte(0);
    return;
  }

  if(address >= 0x080000 && address <= 0x0dffff && io.wramMode == 1) {
    address = io.wramSelect << 17 | (uint17)address;
    wram[address ^ zero] = data.byte(1);
    wram[address ^ size] = data.byte(0);
    return;
  }

  if(address >= 0xfe0000 && address <= 0xfeffff) {
    bram[address >> 1] = data;
    return;
  }

  if(address >= 0xff0000 && address <= 0xff7fff) {
    if(size == Byte && !address.bit(0)) return;
    return pcm.write(address >> 1, data);
  }

  if(address >= 0xff8000 && address <= 0xffffff) {
    return writeIO(size, address, data);
  }

//print("* uw", hex(address), "=", hex(data), "\n");
}
