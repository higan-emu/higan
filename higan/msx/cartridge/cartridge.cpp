#include <msx/msx.hpp>

namespace higan::MSX {

Cartridge cartridge{"Cartridge Slot"};
Cartridge expansion{"Expansion Slot"};
#include "serialization.cpp"

Cartridge::Cartridge(string_view name) : name(name) {
}

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create(name, "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create(interface->name());
  node->load(with);

  information = {};

  if(auto fp = platform->open(node, "metadata.bml", File::Read, File::Required)) {
    information.metadata = fp->reads();
  }

  auto document = BML::unserialize(information.metadata);

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(node, "program.rom", File::Read, File::Required)) {
      rom.load(fp);
    }
  }

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    ram.allocate(memory["size"].natural());
    if(auto fp = platform->open(node, "save.ram", File::Read)) {
      ram.load(fp);
    }
  }

  power();
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  save();
  rom.reset();
  ram.reset();
  node = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(information.metadata);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(auto fp = platform->open(node, "save.ram", File::Write)) {
      ram.save(fp);
    }
  }
}

auto Cartridge::power() -> void {
}

auto Cartridge::read(uint16 address) -> uint8 {
  return rom.read(address);
}

auto Cartridge::write(uint16 address, uint8 data) -> void {
}

}
