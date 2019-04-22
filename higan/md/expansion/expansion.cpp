#include <md/md.hpp>

namespace higan::MegaDrive {

Expansion expansion;
#include "serialization.cpp"

auto Expansion::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Expansion Port", "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto Expansion::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create(interface->name());
  node->load(with);

  information = {};

  if(auto fp = platform->open(node, "metadata.bml", File::Read, File::Required)) {
    information.metadata = fp->reads();
  }

  auto document = BML::unserialize(information.metadata);
  information.region = document["game/region"].text();

  power();
  port->prepend(node);
}

auto Expansion::disconnect() -> void {
  if(!node) return;
  node = {};
}

auto Expansion::save() -> void {
  if(!node) return;
}

auto Expansion::power() -> void {
}

auto Expansion::read(uint22 address) -> uint8 {
  return cdpu.external_read(address);
}

auto Expansion::write(uint22 address, uint8 data) -> void {
  return cdpu.external_write(address, data);
}

auto Expansion::readIO(uint24 address) -> uint8 {
  return cdpu.external_readIO(address);
}

auto Expansion::writeIO(uint24 address, uint8 data) -> void {
  return cdpu.external_writeIO(address, data);
}

}
