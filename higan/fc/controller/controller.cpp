#include <fc/fc.hpp>

namespace higan::Famicom {

#include "port.cpp"
#include "gamepad/gamepad.cpp"

Controller::Controller() {
  if(!handle()) Thread::create(1, [&] {
    while(true) scheduler.resume(), main();
  });
  cpu.peripherals.append(this);
}

Controller::~Controller() {
  cpu.peripherals.removeWhere() == this;
  Thread::destroy();
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

}
