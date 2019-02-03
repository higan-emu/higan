#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

ExpansionPort expansionPort;
#include <sfc/expansion/21fx/21fx.cpp>
#include <sfc/expansion/satellaview/satellaview.cpp>

Expansion::Expansion() {
  if(!handle()) create(Expansion::Enter, 1);
}

Expansion::~Expansion() {
  scheduler.remove(*this);
}

auto Expansion::Enter() -> void {
  while(true) scheduler.synchronize(), expansionPort.device->main();
}

auto Expansion::main() -> void {
  step(1);
  synchronize(cpu);
}

//

auto ExpansionPort::initialize(Node parent) -> void {
  edge = Node::create();
  edge->id = uniqueID();
  edge->edge = true;
  edge->type = "Expansion";
  edge->name = "Expansion Port";
  edge->list.append(Satellaview::create());
  edge->list.append(S21FX::create());
  edge->attach = [&](auto) {
    connect(0);
  };
  parent->nodes.append(edge);
}

auto ExpansionPort::connect(uint deviceID) -> void {
  delete device;
  device = nullptr;
  if(auto leaf = edge->first()) {
    if(leaf->name == "Satellaview") device = new Expansion;
    if(leaf->name == "21fx") device = new S21FX;
  }
  if(!device) device = new Expansion;

  cpu.peripherals.reset();
  if(auto device = controllerPort1.device) cpu.peripherals.append(device);
  if(auto device = controllerPort2.device) cpu.peripherals.append(device);
  if(auto device = expansionPort.device) cpu.peripherals.append(device);
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
