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

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural() >> 1);
    if(auto fp = platform->open(node, "program.rom", File::Read, File::Required)) {
      for(uint address : range(rom.size())) rom.program(address, fp->readm(2));
    }
  }

  power();
  port->prepend(node);
}

auto Expansion::disconnect() -> void {
  if(!node) return;
  rom.reset();
  node = {};
}

auto Expansion::save() -> void {
  if(!node) return;
}

auto Expansion::power() -> void {
}

auto Expansion::read(uint22 address) -> uint16 {
  if(!rom) return 0x0000;
  return rom.read(address >> 1);
}

auto Expansion::write(uint22 address, uint16 data) -> void {
  if(!rom) return;
}

auto Expansion::readIO(uint24 address) -> uint16 {
  static uint16 data;
  data = ~data;
  return data;
}

auto Expansion::writeIO(uint24 address, uint16 data) -> void {
}

}
