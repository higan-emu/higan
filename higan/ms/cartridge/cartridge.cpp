#include <ms/ms.hpp>

namespace higan::MasterSystem {

Cartridge cartridge;
#include "mapper.cpp"
#include "serialization.cpp"

auto Cartridge::initialize(Node::Object parent) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
  port->attach = [&](auto node) {
    load();
  };
  port->detach = [&](auto node) {
    save();
    unload();
  };
  parent->append(port);
}

auto Cartridge::load() -> bool {
  information = {};

  if(auto fp = platform->open(port->connected(), "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return false;

  auto document = BML::unserialize(information.manifest);
  information.title = document["game/label"].text();

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(port->connected(), "program.rom", File::Read, File::Required)) {
      rom.load(fp);
    } else return false;
  }

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    ram.allocate(memory["size"].natural());
    if(memory.nonVolatile) {
      if(auto fp = platform->open(port->connected(), "save.ram", File::Read)) {
        ram.load(fp);
      }
    }
  }

  return true;
}

auto Cartridge::save() -> void {
  auto document = BML::unserialize(information.manifest);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(port->connected(), "save.ram", File::Write)) {
        ram.save(fp);
      }
    }
  }
}

auto Cartridge::unload() -> void {
  rom.reset();
  ram.reset();
}

auto Cartridge::power() -> void {
  mapper = {};
  mapper.romPage0 = 0;
  mapper.romPage1 = 1;
  mapper.romPage2 = 2;
}

}
