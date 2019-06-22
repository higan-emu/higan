auto CPU::idle() -> void {
  cycleEdge();
  step(2);
  status.interruptLatch = status.interruptFlag & status.interruptEnable;
  step(2);
}

auto CPU::read(uint16 address) -> uint8 {
  cycleEdge();
  step(2);
  auto data = bus.read(address);
  status.interruptLatch = status.interruptFlag & status.interruptEnable;
  step(2);
  return data;
}

auto CPU::write(uint16 address, uint8 data) -> void {
  cycleEdge();
  step(2);
  bus.write(address, data);
  status.interruptLatch = status.interruptFlag & status.interruptEnable;
  step(2);
}

auto CPU::cycleEdge() -> void {
  if(r.ei) {
    r.ei = false;
    r.ime = 1;
  }
}

//VRAM DMA source can only be ROM or RAM
auto CPU::readDMA(uint16 address) -> uint8 {
  if(address < 0x8000) return bus.read(address);  //0000-7fff
  if(address < 0xa000) return 0xff;               //8000-9fff
  if(address < 0xe000) return bus.read(address);  //a000-dfff
  return 0xff;                                    //e000-ffff
}

//VRAM DMA target is always VRAM
auto CPU::writeDMA(uint13 address, uint8 data) -> void {
  return bus.write(0x8000 | address, data);
}

auto CPU::readDebugger(uint16 address) -> uint8 {
  return bus.read(address);
}
