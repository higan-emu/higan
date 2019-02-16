#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

APU apu;
#include "serialization.cpp"

auto APU::main() -> void {
  step(1);
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
}

}
