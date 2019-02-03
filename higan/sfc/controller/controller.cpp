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
  case ID::Port::Controller1: bus.write(0x4201, cpu.pio() & ~0x40 | data << 6); break;
  case ID::Port::Controller2: bus.write(0x4201, cpu.pio() & ~0x80 | data << 7); break;
  }
}

//

auto ControllerPort::initialize(Node parent) -> void {
  auto portID = this == &controllerPort2;
  connect();  //temporary hack

  port = Node::Port::create(!portID ? "Controller Port 1" : "Controller Port 2", "Controller");
  port->allocate = [&](auto name) {
    if(name == "Gamepad") return Gamepad::create();
    if(name == "Super Multitap") return SuperMultitap::create();
    return Node::Peripheral::create("Controller");
  };
  port->attach = [&](auto node) {
    connect(node);
  };
  port->detach = [&](auto node) {
    connect();
  };
  parent->append(port);
}

auto ControllerPort::connect(uint deviceID) -> void {
}

auto ControllerPort::connect(Node::Peripheral node) -> void {
  delete device;
  device = nullptr;
  if(node) {
    if(node->name == "Gamepad") device = new Gamepad(node, portID);
//  if(node->name == "Mouse") device = new Mouse(portID);
    if(node->name == "Super Multitap") device = new SuperMultitap(node, portID);
//  if(node->name == "Super Scope") device = new SuperScope(portID);
//  if(node->name == "Justifier") device = new Justifier(portID, false);
//  if(node->name == "Justifiers") device = new Justifier(portID, true);
  }
  if(!device) device = new Controller(portID);
//if(auto peripheral = device->node) port->prepend(peripheral);

  cpu.peripherals.reset();
  if(auto device = controllerPort1.device) cpu.peripherals.append(device);
  if(auto device = controllerPort2.device) cpu.peripherals.append(device);
  if(auto device = expansionPort.device) cpu.peripherals.append(device);
}

auto ControllerPort::power(uint portID) -> void {
  this->portID = portID;
}

auto ControllerPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
}

}
