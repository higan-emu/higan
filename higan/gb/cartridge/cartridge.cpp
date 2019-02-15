#include <gb/gb.hpp>

namespace higan::GameBoy {

Cartridge cartridge;
#include "mbc0/mbc0.cpp"
#include "mbc1/mbc1.cpp"
#include "mbc1m/mbc1m.cpp"
#include "mbc2/mbc2.cpp"
#include "mbc3/mbc3.cpp"
#include "mbc5/mbc5.cpp"
#include "mbc6/mbc6.cpp"
#include "mbc7/mbc7.cpp"
#include "mmm01/mmm01.cpp"
#include "huc1/huc1.cpp"
#include "huc3/huc3.cpp"
#include "tama/tama.cpp"
#include "serialization.cpp"

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  if(!Model::SuperGameBoy()) {
    node = Node::Peripheral::create("Cartridge", port->type);
    node->load(with);
  }

  information = {};
  rom = {};
  ram = {};
  rtc = {};
  mapper = &mbc0;
  accelerometer = false;
  rumble = false;

  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  } else return;

  auto document = BML::unserialize(information.manifest);

  auto mapperID = document["game/board"].text();
  if(mapperID == "MBC0" ) mapper = &mbc0;
  if(mapperID == "MBC1" ) mapper = &mbc1;
  if(mapperID == "MBC1M") mapper = &mbc1m;
  if(mapperID == "MBC2" ) mapper = &mbc2;
  if(mapperID == "MBC3" ) mapper = &mbc3;
  if(mapperID == "MBC5" ) mapper = &mbc5;
  if(mapperID == "MBC6" ) mapper = &mbc6;
  if(mapperID == "MBC7" ) mapper = &mbc7;
  if(mapperID == "MMM01") mapper = &mmm01;
  if(mapperID == "HuC1" ) mapper = &huc1;
  if(mapperID == "HuC3" ) mapper = &huc3;
  if(mapperID == "TAMA" ) mapper = &tama;

  accelerometer = (bool)document["game/board/accelerometer"];
  rumble = (bool)document["game/board/rumble"];

  if(auto memory = Game::Memory{document["game/board/memory(type=ROM,content=Program)"]}) {
    rom.size = max(0x4000, (uint)memory.size);
    rom.data = memory::allocate<uint8>(rom.size, 0xff);
    if(auto fp = platform->open(node, memory.name(), File::Read, File::Required)) {
      fp->read(rom.data, min(rom.size, fp->size()));
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    ram.size = memory.size;
    ram.data = memory::allocate<uint8>(ram.size, 0xff);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Read, File::Optional)) {
        fp->read(ram.data, min(ram.size, fp->size()));
      }
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    rtc.size = memory.size;
    rtc.data = memory::allocate<uint8>(rtc.size, 0xff);
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Read, File::Optional)) {
        fp->read(rtc.data, min(rtc.size, fp->size()));
      }
    }
  }

  information.sha256 = Hash::SHA256({rom.data, rom.size}).digest();
  mapper->load(document);

  power();
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  delete[] rom.data;
  delete[] ram.data;
  delete[] rtc.data;
  rom = {};
  ram = {};
  rtc = {};
  node = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(information.manifest);

  if(auto memory = Game::Memory{document["game/board/memory(type=RAM,content=Save)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Write)) {
        fp->write(ram.data, ram.size);
      }
    }
  }

  if(auto memory = Game::Memory{document["game/board/memory(type=RTC,content=Time)"]}) {
    if(memory.nonVolatile) {
      if(auto fp = platform->open(node, memory.name(), File::Write)) {
        fp->write(rtc.data, rtc.size);
      }
    }
  }

  mapper->save(document);
}

auto Cartridge::power() -> void {
  Thread::create(4 * 1024 * 1024, [&] {
    while(true) scheduler.synchronize(), main();
  });

  for(uint n = 0x0000; n <= 0x7fff; n++) bus.mmio[n] = this;
  for(uint n = 0xa000; n <= 0xbfff; n++) bus.mmio[n] = this;
  bus.mmio[0xff50] = this;

  bootromEnable = true;

  if(mapper) mapper->power();
}

auto Cartridge::main() -> void {
  mapper->main();
}

auto Cartridge::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto Cartridge::second() -> void {
  mapper->second();
}

auto Cartridge::readIO(uint16 address) -> uint8 {
  if(address == 0xff50) return 0xff;
  if(bootromEnable) {
    if(address >= 0x0000 && address <= 0x00ff) return system.bootROM.read(address);
    if(address >= 0x0200 && address <= 0x08ff && Model::GameBoyColor()) return system.bootROM.read(address - 0x100);
  }
  return mapper->read(address);
}

auto Cartridge::writeIO(uint16 address, uint8 data) -> void {
  if(bootromEnable && address == 0xff50) return void(bootromEnable = false);
  mapper->write(address, data);
}

auto Cartridge::Memory::read(uint address) const -> uint8 {
  if(!size) return 0xff;
  if(address >= size) address %= size;
  return data[address];
}

auto Cartridge::Memory::write(uint address, uint8 byte) -> void {
  if(!size) return;
  if(address >= size) address %= size;
  data[address] = byte;
}

//

auto Cartridge::Mapper::main() -> void {
  cartridge.step(cartridge.frequency());
}

}
