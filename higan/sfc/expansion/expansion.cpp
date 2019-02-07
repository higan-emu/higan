#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ExpansionPort expansionPort{"Expansion Port"};
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

//

auto ExpansionPort::create(string_view name) -> Node::Port {
  return Node::Port::create(name, "Expansion");
}

ExpansionPort::ExpansionPort(string_view name) : name(name) {
}

auto ExpansionPort::initialize(Node::Object parent) -> void {
  bind(port = create(name));
  parent->append(port);
}

auto ExpansionPort::bind(Node::Port port) -> void {
  this->port = port;
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
}

auto ExpansionPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Satellaview") device = new Satellaview(node);
    if(node->name == "21fx") device = new S21FX(node);
  }
}

auto ExpansionPort::disconnect() -> void {
  delete device;
  device = nullptr;
}

auto ExpansionPort::serialize(serializer& s) -> void {
}

}
