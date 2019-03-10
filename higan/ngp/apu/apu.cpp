#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

APU apu;
#include "memory.cpp"
#include "serialization.cpp"

auto APU::main() -> void {
  if(!io.enable) return step(16);

  if(nmi.line) {
    nmi.line = 0;  //edge-sensitive
    irq(0, 0x0066, 0xff);
  }

//static uint ctr=0;
//if(++ctr<4000) print(disassemble(r.pc), "\n");

  instruction();
}

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto APU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto APU::power() -> void {
  Z80::bus = this;
  Z80::power();
  bus->grant(false);
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  ram.allocate(0x1000);
  nmi = {};
  io = {};
}

auto APU::enable() -> void {
  Thread::destroy();
  Z80::power();
  bus->grant(false);
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });
  io.enable = true;
}

auto APU::disable() -> void {
  io.enable = false;
}

}
