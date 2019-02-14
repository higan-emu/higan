#include <ms/ms.hpp>

namespace higan::MasterSystem {

#include "port.cpp"
#include "numberpad/numberpad.cpp"
#include "gamepad/gamepad.cpp"

Controller::Controller() {
  if(!handle()) create(1, [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.peripherals.append(this);
}

Controller::~Controller() {
  cpu.peripherals.removeValue(this);
  Thread::destroy();
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
