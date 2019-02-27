SufamiTurboCartridge sufamiturboA;
SufamiTurboCartridge sufamiturboB;
#include "memory.cpp"
#include "serialization.cpp"

auto SufamiTurboCartridge::load(Node::Peripheral parent, Node::Peripheral from) -> void {
  bool portID = this == &sufamiturboB;
  port = Node::Port::create(string{"Sufami Turbo Cartridge Slot ", !portID ? "A" : "B"}, "Sufami Turbo Cartridge");
  port->allocate = [&] { return Node::Peripheral::create("Sufami Turbo"); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto SufamiTurboCartridge::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create("Sufami Turbo");
  node->load(with);

  if(auto fp = platform->open(node, "metadata.bml", File::Read, File::Required)) {
    self.metadata = fp->reads();
  } else return;

  auto document = BML::unserialize(self.metadata);

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

  port->prepend(node);
}

auto SufamiTurboCartridge::disconnect() -> void {
  if(!node) return;
  save();
  rom.reset();
  ram.reset();
  node = {};
}

auto SufamiTurboCartridge::power() -> void {
}

auto SufamiTurboCartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(self.metadata);

  if(auto memory = document["game/board/memory(type=RAM,content=Save)"]) {
    if(!(bool)memory["volatile"]) {
      if(auto fp = platform->open(node, "save.ram", File::Write)) {
        fp->write(ram.data(), ram.size());
      }
    }
  }
}
