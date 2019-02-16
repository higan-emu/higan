#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

CPU cpu;
#include "memory.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
static uint ctr=0;
if(++ctr<200) print(disassemble(), "\n");
  instruction();
  step(1);
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(vpu);
  synchronize(apu);
  synchronize(psg);
}

auto CPU::power() -> void {
  TLCS900H::power();
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  ram.allocate(0x3000);
  r.pc.l.l0 = 0xff1800;
  io = {};
}

auto CPU::setInterruptHblank(boolean line) -> void {
  io.irq.hblank = line;
//if(line) interrupt(0xffff0c);
}

auto CPU::setInterruptVblank(boolean line) -> void {
  io.irq.vblank = line;
  if(line) interrupt(0xffff10);
}

}
