auto VCE::CRAM::read(uint9 address) -> uint9 {
  return data[address];
}

auto VCE::CRAM::write(uint9 address, bool a0, uint8 value) -> void {
  if(!a0) {
    data[address].bit(0,7) = value.bit(0,7);
  } else {
    data[address].bit(8) = value.bit(0);
  }
}
