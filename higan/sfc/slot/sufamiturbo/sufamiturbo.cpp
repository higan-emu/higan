SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;
#include "memory.cpp"
#include "serialization.cpp"

auto SufamiTurboCartridge::create(bool slot) -> Node::Peripheral {
  auto node = Node::Peripheral::create("Sufami Turbo Cartridges");
  return node;
}

auto SufamiTurboCartridge::initialize(Node parent) -> void {
  bool portID = this == &sufamiturboB;
  port = Node::Port::create(!portID ? "Sufami Turbo Slot A" : "Sufami Turbo Slot B", "Sufami Turbo Cartridge");
/*
  port->attach = [&](auto node) {
    load();
  };
  port->detach = [&](auto node) {
    unload();
  };
*/
  parent->append(port);
}

auto SufamiTurboCartridge::load() -> void {
  if(auto fp = platform->open(port->connected(), "manifest.bml", File::Read, File::Required)) {
    self.manifest = fp->reads();
  } else return;

  auto document = BML::unserialize(self.manifest);
  port->connected()->name = document["game/label"].text();

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(port->connected(), "program.rom", File::Read, File::Required)) {
      fp->read(rom.data(), rom.size());
    }
  }

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    ram.allocate(memory["size"].natural());
    if(!(bool)memory["volatile"]) {
      if(auto fp = platform->open(port->connected(), "save.ram", File::Read)) {
        fp->read(ram.data(), ram.size());
      }
    }
  }
}

auto SufamiTurboCartridge::save() -> void {
  auto document = BML::unserialize(self.manifest);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(!(bool)memory["volatile"]) {
      if(auto fp = platform->open(port->connected(), "save.ram", File::Write)) {
        fp->write(ram.data(), ram.size());
      }
    }
  }
}

auto SufamiTurboCartridge::unload() -> void {
  save();
  rom.reset();
  ram.reset();
}

auto SufamiTurboCartridge::power() -> void {
}
