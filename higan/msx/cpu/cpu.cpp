#include <msx/msx.hpp>

namespace higan::MSX {

CPU cpu;
#include "memory.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
  if(io.irqLine) irq(1, 0x0038, 0xff);
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  while(!Thread::synchronize());
}

auto CPU::power() -> void {
  Z80::bus = this;
  Z80::power();
  Thread::create(system.colorburst(), {&CPU::main, this});

  r.pc = 0x0000;  //reset vector address

  if(Model::MSX() ) ram.allocate (64_KiB);
  if(Model::MSX2()) ram.allocate(256_KiB);

  slot[0] = {3, 0, {0, 0, 0, 0}};
  slot[1] = {2, 1, {0, 0, 0, 0}};
  slot[2] = {1, 2, {0, 0, 0, 0}};
  slot[3] = {0, 3, {0, 0, 0, 0}};

  io = {};
}

auto CPU::setIRQ(bool line) -> void {
  io.irqLine = line;
}

}
