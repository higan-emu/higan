auto HuC6280::load8(uint8 address) -> uint8 {
  step(r.cs);
  return read(r.mpr[1], address);
}

auto HuC6280::load16(uint16 address) -> uint8 {
  step(r.cs);
  return read(r.mpr[address.bit(13,15)], address.bit(0,12));
}

auto HuC6280::store8(uint8 address, uint8 data) -> void {
  step(r.cs);
  return write(r.mpr[1], address, data);
}

auto HuC6280::store16(uint16 address, uint8 data) -> void {
  step(r.cs);
  return write(r.mpr[address.bit(13,15)], address.bit(0,12), data);
}

//

auto HuC6280::io() -> void {
  step(r.cs);
}

auto HuC6280::opcode() -> uint8 {
  return load16(PC++);
}

auto HuC6280::operand() -> uint8 {
  return load16(PC++);
}

//

auto HuC6280::push(uint8 data) -> void {
  step(r.cs);
  write(r.mpr[1], 0x0100 | S, data);
  S--;
}

auto HuC6280::pull() -> uint8 {
  step(r.cs);
  S++;
  return read(r.mpr[1], 0x0100 | S);
}
