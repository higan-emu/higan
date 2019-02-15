#include <md/md.hpp>

namespace higan::MegaDrive {

#include "port.cpp"
#include "control-pad/control-pad.cpp"
#include "fighting-pad/fighting-pad.cpp"

Controller::Controller() {
  if(!handle()) Thread::create(1, [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.peripherals.append(this);
}

Controller::~Controller() {
  removeWhere(cpu.peripherals) == this;
  Thread::destroy();
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
