#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ExpansionPort expansionPort;
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
  scheduler.remove(*this);
}

auto Expansion::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ExpansionPort::initialize(Node::Object parent) -> void {
  port = Node::Port::create("Expansion Port", "Expansion");
/*
  port->attach = [&](auto) {
    connect(0);
  };
*/
  parent->append(port);
}

auto ExpansionPort::connect(uint deviceID) -> void {
  delete device;
  device = nullptr;
//  if(auto leaf = port->first()) {
//    if(leaf->name == "Satellaview") device = new Expansion;
//    if(leaf->name == "21fx") device = new S21FX;
//  }
  if(!device) device = new Expansion;
}

auto ExpansionPort::power() -> void {
}

auto ExpansionPort::unload() -> void {
  delete device;
  device = nullptr;
}

auto ExpansionPort::serialize(serializer& s) -> void {
}

}
