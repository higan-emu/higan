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

ExpansionPort::ExpansionPort(string_view name) : name(name) {
}

auto ExpansionPort::load(Node::Object parent, Node::Object from) -> void {
  parent->append(port = Node::Port::create(name, "Expansion"));
  port->attach = [&](auto node) { connect(node); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
}

auto ExpansionPort::connect(Node::Peripheral node) -> void {
  disconnect();
  if(node) {
    if(node->name == "Satellaview") device = new Satellaview(port, node);
    if(node->name == "21fx") device = new S21FX(port, node);
  }
}

auto ExpansionPort::disconnect() -> void {
  port->disconnect();
  device = {};
}

auto ExpansionPort::serialize(serializer& s) -> void {
}

}
