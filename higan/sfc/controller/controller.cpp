#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ControllerPort controllerPort1{"Controller Port 1"};
ControllerPort controllerPort2{"Controller Port 2"};
#include "gamepad/gamepad.cpp"
//#include "justifier/justifier.cpp"
//#include "justifiers/justifiers.cpp"
//#include "mouse/mouse.cpp"
//#include "ntt-data-keypad/ntt-data-keypad.cpp"
//#include "super-multitap/super-multitap.cpp"
//#include "super-scope/super-scope.cpp"
//#include "twin-tap/twin-tap.cpp"

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
  if(co_active() == controllerPort1.device.data()) return cpu.pio() & 0x40;
  if(co_active() == controllerPort2.device.data()) return cpu.pio() & 0x80;
  return 1;
}

auto Controller::iobit(bool data) -> void {
  if(co_active() == controllerPort1.device.data()) bus.write(0x4201, cpu.pio() & ~0x40 | data << 6);
  if(co_active() == controllerPort2.device.data()) bus.write(0x4201, cpu.pio() & ~0x80 | data << 7);
}

//

auto ControllerPort::create(string_view name) -> Node::Port {
  auto port = Node::Port::create(name, "Controller");
  port->hotSwappable = true;
//  port->allocate = [&](auto name) {
//    if(name == "Gamepad") return Tree::Gamepad::create();
//    if(name == "Justifier") return Justifier::create();
//    if(name == "Justifiers") return Justifiers::create();
//    if(name == "Mouse") return Mouse::create();
//    if(name == "NTT Data Keypad") return NTTDataKeypad::create();
//    if(name == "Super Multitap") return SuperMultitap::create();
//    if(name == "Super Scope") return SuperScope::create();
//    if(name == "Twin Tap") return TwinTap::create();
//    return Node::Peripheral::create("Controller");
//  };
  return port;
}

ControllerPort::ControllerPort(string_view name) : name(name) {
}

auto ControllerPort::load(Node::Object parent, Node::Object from) -> void {
  parent->append(port = Node::Port::create(name, "Controller"));
  port->attach = [&](auto node) { connect(node); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
}

auto ControllerPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Gamepad") device = new Gamepad(port, node);
//    if(node->name == "Justifier") device = new Justifier(node);
//    if(node->name == "Justifiers") device = new Justifiers(node);
//    if(node->name == "Mouse") device = new Mouse(node);
//    if(node->name == "NTT Data Keypad") device = new NTTDataKeypad(node);
//    if(node->name == "Super Multitap") device = new SuperMultitap(node);
//    if(node->name == "Super Scope") device = new SuperScope(node);
//    if(node->name == "Twin Tap") device = new TwinTap(node);
  }
}

auto ControllerPort::disconnect() -> void {
  port->disconnect();
  device = {};
}

auto ControllerPort::serialize(serializer& s) -> void {
}

}
