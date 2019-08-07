auto VDC::VRAM::read(uint16 address) -> uint16 {
  if(address.bit(15)) return 0x0000;  //todo: random data?
  return data[address];
}

auto VDC::VRAM::write(uint16 address, uint16 value) -> void {
  if(address.bit(15)) return;
  data[address] = value;
}

auto VDC::SATB::read(uint8 address) -> uint16 {
  return data[address];
}

auto VDC::SATB::write(uint8 address, uint16 value) -> void {
  data[address] = value;
}
