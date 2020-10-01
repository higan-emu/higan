inline auto CPU::readByte(u32 address) -> u32 {
  if(scc.status.cache.isolate) return icache.readByte(address);
  return bus.readByte(address);
}

inline auto CPU::readHalf(u32 address) -> u32 {
  if(scc.status.cache.isolate) return icache.readHalf(address);
  return bus.readHalf(address);
}

inline auto CPU::readWord(u32 address) -> u32 {
  if(scc.status.cache.isolate) return icache.readWord(address);
  return bus.readWord(address);
}

inline auto CPU::writeByte(u32 address, u32 data) -> void {
  if(scc.status.cache.isolate) return icache.writeByte(address, data);
  return bus.writeByte(address, data);
}

inline auto CPU::writeHalf(u32 address, u32 data) -> void {
  if(scc.status.cache.isolate) return icache.writeHalf(address, data);
  return bus.writeHalf(address, data);
}

inline auto CPU::writeWord(u32 address, u32 data) -> void {
  if(scc.status.cache.isolate) return icache.writeWord(address, data);
  return bus.writeWord(address, data);
}
