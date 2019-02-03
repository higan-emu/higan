#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ControllerPort controllerPort1;
ControllerPort controllerPort2;
#include "gamepad/gamepad.cpp"
#include "mouse/mouse.cpp"
#include "super-multitap/super-multitap.cpp"
#include "super-scope/super-scope.cpp"
#include "justifier/justifier.cpp"

Controller::Controller(uint port) : port(port) {
  if(!handle()) create(Controller::Enter, 1);
}

Controller::~Controller() {
  scheduler.remove(*this);
}

auto Controller::Enter() -> void {
  while(true) {
    scheduler.synchronize();
    if(controllerPort1.device->active()) controllerPort1.device->main();
    if(controllerPort2.device->active()) controllerPort2.device->main();
  }
}

auto Controller::main() -> void {
  step(1);
  synchronize(cpu);
}

auto Controller::iobit() -> bool {
  switch(port) {
  case ID::Port::Controller1: return cpu.pio() & 0x40;
  case ID::Port::Controller2: return cpu.pio() & 0x80;
  }
  unreachable;
}

auto Controller::iobit(bool data) -> void {
  switch(port) {
  case ID::Port::Controller1: bus.write(0x4201, (cpu.pio() & ~0x40) | (data << 6)); break;
  case ID::Port::Controller2: bus.write(0x4201, (cpu.pio() & ~0x80) | (data << 7)); break;
  }
}

//

auto ControllerPort::initialize(Node parent) -> void {
  edge = Node::create();
  edge->id = uniqueID();
  edge->edge = true;
  edge->type = "Controller";
  edge->attach = [&](auto) {
    connect(0);
  };
  vector<string> controllers;
  if(this == &controllerPort1) {
    edge->name = "Controller Port 1";
    edge->list.append(Gamepad::create());
    edge->list.append(Mouse::create());
    edge->list.append(SuperMultitap::create());
  } else {
    edge->name = "Controller Port 2";
    edge->list.append(Gamepad::create());
    edge->list.append(Mouse::create());
    edge->list.append(SuperMultitap::create());
    edge->list.append(SuperScope::create());
    edge->list.append(Justifier::create(0));
    edge->list.append(Justifier::create(1));
  }
  parent->nodes.append(edge);
}

auto ControllerPort::connect(uint deviceID) -> void {
  delete device;
  device = nullptr;
  if(auto leaf = edge->first()) {
    if(leaf->name == "Gamepad") device = new Gamepad(port);
    if(leaf->name == "Mouse") device = new Mouse(port);
    if(leaf->name == "Super Multitap") device = new SuperMultitap(port);
    if(leaf->name == "Super Scope") device = new SuperScope(port);
    if(leaf->name == "Justifier") device = new Justifier(port, false);
    if(leaf->name == "Justifiers") device = new Justifier(port, true);
  }
  if(!device) device = new Controller(port);

  cpu.peripherals.reset();
  if(auto device = controllerPort1.device) cpu.peripherals.append(device);
  if(auto device = controllerPort2.device) cpu.peripherals.append(device);
  if(auto device = expansionPort.device) cpu.peripherals.append(device);
}

auto ControllerPort::power(uint port) -> void {
  this->port = port;
}

auto ControllerPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
}

}
