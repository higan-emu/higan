#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ControllerPort controllerPort1{"Controller Port 1"};
ControllerPort controllerPort2{"Controller Port 2"};
#include "gamepad/gamepad.cpp"
#include "mouse/mouse.cpp"
#include "super-multitap/super-multitap.cpp"
#include "super-scope/super-scope.cpp"
//#include "justifier/justifier.cpp"

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

auto Controller::iobit() -> bool {
  if(co_active() == controllerPort1.device) return cpu.pio() & 0x40;
  if(co_active() == controllerPort2.device) return cpu.pio() & 0x80;
  return 1;
}

auto Controller::iobit(bool data) -> void {
  if(co_active() == controllerPort1.device) bus.write(0x4201, cpu.pio() & ~0x40 | data << 6);
  if(co_active() == controllerPort2.device) bus.write(0x4201, cpu.pio() & ~0x80 | data << 7);
}

//

auto ControllerPort::create(string_view name) -> Node::Port {
  auto port = Node::Port::create(name, "Controller");
  port->hotSwappable = true;
  port->allocate = [&](auto name) {
    if(name == "Gamepad") return Gamepad::create();
    if(name == "Mouse") return Mouse::create();
    if(name == "Super Multitap") return SuperMultitap::create();
    if(name == "Super Scope") return SuperScope::create();
  //if(name == "Justifier") return Justifier::create();
    return Node::Peripheral::create("Controller");
  };
  return port;
}

ControllerPort::ControllerPort(string_view name) : name(name) {
}

auto ControllerPort::initialize(Node::Object parent) -> void {
  bind(port = create(name));
  parent->append(port);
}

auto ControllerPort::bind(Node::Port port) -> void {
  this->port = port;
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
}

auto ControllerPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Gamepad") device = new Gamepad(node);
    if(node->name == "Mouse") device = new Mouse(node);
    if(node->name == "Super Multitap") device = new SuperMultitap(node);
    if(node->name == "Super Scope") device = new SuperScope(node);
  //if(node->name == "Justifier") device = new Justifier(node);
  }
}

auto ControllerPort::disconnect() -> void {
  delete device;
  device = nullptr;
}

auto ControllerPort::serialize(serializer& s) -> void {
}

}
