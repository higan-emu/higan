NECDSP necdsp;
#include "serialization.cpp"

auto NECDSP::main() -> void {
  exec();
  Thread::step(1);
  Thread::synchronize(cpu);
}

auto NECDSP::read(uint24 address, uint8) -> uint8 {
  cpu.synchronize(*this);
  if(address.bit(0)) {
    return uPD96050::readSR();
  } else {
    return uPD96050::readDR();
  }
}

auto NECDSP::write(uint24 address, uint8 data) -> void {
  cpu.synchronize(*this);
  if(address.bit(0)) {
    return uPD96050::writeSR(data);
  } else {
    return uPD96050::writeDR(data);
  }
}

auto NECDSP::readRAM(uint24 address, uint8) -> uint8 {
  cpu.synchronize(*this);
  return uPD96050::readDP(address);
}

auto NECDSP::writeRAM(uint24 address, uint8 data) -> void {
  cpu.synchronize(*this);
  return uPD96050::writeDP(address, data);
}

auto NECDSP::unload() -> void {
  cpu.coprocessors.removeByValue(this);
  Thread::destroy();
}

auto NECDSP::power() -> void {
  uPD96050::power();
  Thread::create(Frequency, {&NECDSP::main, this});
  cpu.coprocessors.append(this);
}
