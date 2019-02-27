#include <ws/ws.hpp>

namespace higan::WonderSwan {

Cartridge cartridge;
#include "memory.cpp"
#include "rtc.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto Cartridge::main() -> void {
  if(rtc.data) {
    rtcTickSecond();
    rtcCheckAlarm();
  }
  step(3'072'000);
}

auto Cartridge::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
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
  } else return;

  auto document = BML::unserialize(information.metadata);

  if(auto memory = Game::Memory{document["game/board/memory(type=ROM,content=Program)"]}) {
    rom.size = memory.size;
    rom.mask = bit::round(rom.size) - 1;
    rom.data = new uint8[rom.mask + 1];
    memory::fill<uint8>(rom.data, rom.mask + 1, 0xff);
    if(auto fp = platform->open(node, memory.name(), File::Read, File::Required)) {
      fp->read(rom.data, rom.size);
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    ram.size = memory.size;
    ram.mask = bit::round(ram.size) - 1;
    ram.data = new uint8[ram.mask + 1];
    memory::fill<uint8>(ram.data, ram.mask + 1, 0xff);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Read)) {
        fp->read(ram.data, ram.size);
      }
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    eeprom.setSize(memory.size / sizeof(uint16));
    eeprom.erase();
    if(auto fp = platform->open(node, memory.name(), File::Read)) {
      fp->read(eeprom.data(), eeprom.size());
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    rtc.size = memory.size;
    rtc.mask = bit::round(rtc.size) - 1;
    rtc.data = new uint8[rtc.mask + 1];
    memory::fill<uint8>(rtc.data, rtc.mask + 1, 0x00);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Read)) {
        fp->read(rtc.data, rtc.size);
      }
    }
  }

  if(document["game/orientation"].text() == "horizontal") information.orientation = "Horizontal";
  if(document["game/orientation"].text() == "vertical"  ) information.orientation = "Vertical";

  power();
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  Thread::destroy();

  delete[] rom.data;
  rom.data = nullptr;
  rom.size = 0;
  rom.mask = 0;

  delete[] ram.data;
  ram.data = nullptr;
  ram.size = 0;
  ram.mask = 0;

  delete[] rtc.data;
  rtc.data = nullptr;
  rtc.size = 0;
  rtc.mask = 0;
}

auto Cartridge::save() -> void {
  if(!node) return;

  auto document = BML::unserialize(information.metadata);

  if(auto memory = Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Write)) {
        fp->write(ram.data, ram.size);
      }
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=EEPROM,content=Save)"]}) {
    if(auto fp = platform->open(node, memory.name(), File::Write)) {
      fp->write(eeprom.data(), eeprom.size());
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Write)) {
        fp->write(rtc.data, rtc.size);
      }
    }
  }
}

auto Cartridge::power() -> void {
  Thread::create(3'072'000, [&] {
    while(true) scheduler.synchronize(), main();
  });
  eeprom.power();

  bus.map(this, 0x00c0, 0x00c8);
  if(rtc.data) bus.map(this, 0x00ca, 0x00cb);
  bus.map(this, 0x00cc, 0x00cd);

  r = {};
}

}
