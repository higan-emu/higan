SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;
#include "memory.cpp"
#include "serialization.cpp"

auto SufamiTurboCartridge::initialize(Node::Object parent) -> void {
  bool portID = this == &sufamiturboB;
  port = Node::Port::create(string{"Sufami Turbo Slot ", !portID ? "A" : "B"}, "Sufami Turbo");
/*
  port->allocate = [&](auto name) {
    return Node::Peripheral::create("Sufami Turbo");
  };
  port->attach = [&](auto node) {
    load();
  };
  port->detach = [&](auto node) {
    save();
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
