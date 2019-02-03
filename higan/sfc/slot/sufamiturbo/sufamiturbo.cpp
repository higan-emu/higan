SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;
#include "memory.cpp"
#include "serialization.cpp"

auto SufamiTurboCartridge::create(bool slot) -> Node {
  auto node = Node::create();
  node->id = uniqueID();
  node->type = "Cartridge";
  node->name = {"Sufami Turbo - ", !slot ? "Slot A" : "Slot B"};
  return node;
}

auto SufamiTurboCartridge::initialize(Node parent) -> void {
  edge = Node::create();
  edge->id = uniqueID();
  edge->edge = true;
  edge->type = "Cartridge";
  if(this == &sufamiturboA) {
    edge->name = "Sufami Turbo - Slot A";
  } else {
    edge->name = "Sufami Turbo - Slot B";
  }
  edge->attach = [&](auto node) {
    this->node = node;
    load();
  };
  edge->detach = [&](auto node) {
    unload();
    this->node = {};
  };
  parent->append(edge);
}

auto SufamiTurboCartridge::load() -> void {
  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    self.manifest = fp->reads();
  } else return;

  auto document = BML::unserialize(self.manifest);
  node->name = document["game/label"].text();

  if(auto memory = document["game/board/memory(type=ROM,content=Program)"]) {
    rom.allocate(memory["size"].natural());
    if(auto fp = platform->open(node, "program.rom", File::Read, File::Required)) {
      fp->read(rom.data(), rom.size());
    }
  }

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    ram.allocate(memory["size"].natural());
    if(!(bool)memory["volatile"]) {
      if(auto fp = platform->open(node, "save.ram", File::Read)) {
        fp->read(ram.data(), ram.size());
      }
    }
  }
}

auto SufamiTurboCartridge::save() -> void {
  auto document = BML::unserialize(self.manifest);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(!(bool)memory["volatile"]) {
      if(auto fp = platform->open(node, "save.ram", File::Write)) {
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
