//these functions transform internal accesses to bus accesses, and handle conversions:
//* A1-A23 are passed to the bus
//* A0 in word mode is ignored, and drives both UDS and LDS
//* A0 in byte mode drives either UDS or LDS
//* upper = /UDS (1 = selected; eg /UDS is inverted)
//* lower = /LDS (1 = selected; eg /LDS is inverted)
//* /UDS is where A0=0 and maps to D8-D15
//* /LDS is where A0=1 and maps to D0-D7

template<> auto M68K::read<Byte>(uint32 address) -> uint32 {
  step(4);
  if(address & 1) {
    return read(0, 1, address & ~1).byte(0);  /* /LDS */
  } else {
    return read(1, 0, address & ~1).byte(1);  /* /UDS */
  }
}

template<> auto M68K::read<Word>(uint32 address) -> uint32 {
  step(4);
  return read(1, 1, address & ~1);
}

template<> auto M68K::read<Long>(uint32 address) -> uint32 {
  step(4);
  uint32 data = read(1, 1, address + 0 & ~1) << 16;
  step(4);
  return data | read(1, 1, address + 2 & ~1) <<  0;
}

//

template<> auto M68K::write<Byte>(uint32 address, uint32 data) -> void {
  step(4);
  if(address & 1) {
    return write(0, 1, address & ~1, data << 8 | data << 0);  /* /LDS */
  } else {
    return write(1, 0, address & ~1, data << 8 | data << 0);  /* /UDS */
  }
}

template<> auto M68K::write<Word>(uint32 address, uint32 data) -> void {
  step(4);
  return write(1, 1, address & ~1, data);
}

template<> auto M68K::write<Long>(uint32 address, uint32 data) -> void {
  step(4);
  write(1, 1, address + 0 & ~1, data >> 16);
  step(4);
  write(1, 1, address + 2 & ~1, data >>  0);
}

//

template<> auto M68K::write<Byte, Reverse>(uint32 address, uint32 data) -> void {
  step(4);
  if(address & 1) {
    return write(0, 1, address & ~1, data << 8 | data << 0);  /* /LDS */
  } else {
    return write(1, 0, address & ~1, data << 8 | data << 0);  /* /UDS */
  }
}

template<> auto M68K::write<Word, Reverse>(uint32 address, uint32 data) -> void {
  step(4);
  return write(1, 1, address & ~1, data);
}

template<> auto M68K::write<Long, Reverse>(uint32 address, uint32 data) -> void {
  step(4);
  write(1, 1, address + 2 & ~1, data >>  0);
  step(4);
  write(1, 1, address + 0 & ~1, data >> 16);
}

//

template<> auto M68K::readPC<Byte>() -> uint32 {
  step(4);
  auto data = read(1, 1, r.pc & ~1);
  r.pc += 2;
  return data;
}

template<> auto M68K::readPC<Word>() -> uint32 {
  step(4);
  auto data = read(1, 1, r.pc & ~1);
  r.pc += 2;
  return data;
}

template<> auto M68K::readPC<Long>() -> uint32 {
  step(4);
  auto hi = read(1, 1, r.pc & ~1);
  r.pc += 2;
  step(4);
  auto lo = read(1, 1, r.pc & ~1);
  r.pc += 2;
  return hi << 16 | lo << 0;
}

//

template<uint Size> auto M68K::pop() -> uint32 {
  auto data = read<Size>((uint32)r.a[7]);
  r.a[7] += bytes<Size>();
  return data;
}

template<uint Size> auto M68K::push(uint32 data) -> void {
  r.a[7] -= bytes<Size>();
  return write<Size, Reverse>((uint32)r.a[7], data);
}
