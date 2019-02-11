#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

#include <sfc/expansion/port.cpp>
#include <sfc/expansion/21fx/21fx.cpp>
#include <sfc/expansion/satellaview/satellaview.cpp>

Expansion::Expansion() {
  if(!handle()) create(1, [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.peripherals.append(this);
}

Expansion::~Expansion() {
  cpu.peripherals.removeValue(this);
  Thread::destroy();
}

auto Expansion::main() -> void {
  step(1);
  synchronize(cpu);
}

}
