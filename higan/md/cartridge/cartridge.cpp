#include <md/md.hpp>

namespace higan::MegaDrive {

Cartridge cartridge;
#include "serialization.cpp"

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
  }

  auto document = BML::unserialize(information.metadata);

  if(!loadROM(rom, document["game/board/memory(type=ROM,content=Program)"])) {
    rom.reset();
  }

  if(!loadROM(patch, document["game/board/memory(type=ROM,content=Patch)"])) {
    patch.reset();
  }

  if(!loadRAM(ram, document["game/board/memory(type=RAM,content=Save)"])) {
    ram.reset();
  }

  information.region = document["game/region"].text();

  read = {&Cartridge::readLinear, this};
  write = {&Cartridge::writeLinear, this};

  if(rom.size() > 0x200000) {
    read = {&Cartridge::readBanked, this};
    write = {&Cartridge::writeBanked, this};
  }

  if(document["game/board/slot(type=Mega Drive)"]) {
    slot = new Cartridge{depth + 1};
    slot->load(node, with);

    if(patch) {
      read = {&Cartridge::readLockOn, this};
      write = {&Cartridge::writeLockOn, this};
    } else {
      read = {&Cartridge::readGameGenie, this};
      write = {&Cartridge::writeGameGenie, this};
    }
  }

  //easter egg: power draw increases with each successively stacked cartridge
  //simulate increasing address/data line errors as stacking increases
  //todo: this was written before adding the size parameter; words were always assumed before
  //code needs to be ported to take the size parameter into account before being enabled again
  #if 0
  if(depth >= 3) {
    auto reader = read;
    auto writer = write;
    auto scramble = [=](uint32 value) -> uint32 {
      uint chance = max(1, (1 << 19) >> depth) - 1;
      if((random() & chance) == 0) value ^= 1 << (random() & 31);
      return value;
    };
    read = [=](uint1 size, uint22 address, uint16 data) -> uint16 {
      return scramble(reader(size, scramble(address), data));
    };
    write = [=](uint1 size, uint22 address, uint16 data) -> void {
      writer(size, scramble(address), scramble(data));
    };
  }
  #endif

  power();
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  rom.reset();
  patch.reset();
  ram.reset();
  read.reset();
  write.reset();
  if(slot) slot->disconnect();
  slot.reset();
  node = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(information.metadata);
  saveRAM(ram, document["game/board/memory(type=RAM,content=Save)"]);
  if(slot) slot->save();
}

auto Cartridge::power() -> void {
  ramEnable = 1;
  ramWritable = 1;
  for(uint n : range(8)) romBank[n] = n;
  gameGenie = {};
}

//

auto Cartridge::loadROM(Memory::Readable<uint8>& rom, Markup::Node memory) -> bool {
  if(!memory) return false;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  rom.allocate(memory["size"].natural());
  if(auto fp = platform->open(node, name, File::Read, File::Required)) {
    rom.load(fp);
  };

  return true;
}

auto Cartridge::loadRAM(Memory::Writable<uint8>& ram, Markup::Node memory) -> bool {
  if(!memory) return false;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  if(auto mode = memory["mode"].text()) {
    if(mode == "lo"  ) ramBits = 0x00ff;
    if(mode == "hi"  ) ramBits = 0xff00;
    if(mode == "word") ramBits = 0xffff;
  }
  ram.allocate(memory["size"].natural());
  if(!memory["volatile"]) {
    if(auto fp = platform->open(node, name, File::Read)) {
      ram.load(fp);
    }
  }

  return true;
}

auto Cartridge::saveRAM(Memory::Writable<uint8>& ram, Markup::Node memory) -> bool {
  if(!memory) return false;
  if(memory["volatile"]) return true;

  auto name = string{memory["content"].text(), ".", memory["type"].text()}.downcase();
  if(auto fp = platform->open(node, name, File::Write)) {
    ram.save(fp);
  } else return false;

  return true;
}

//

auto Cartridge::readIO(uint1 size, uint24 address, uint16 data) -> uint16 {
  if(slot) slot->readIO(size, address, data);
  return data;
}

auto Cartridge::writeIO(uint1 size, uint24 address, uint16 data) -> void {
  writeIO(address, data.byte(size));
  if(size == Word) writeIO(address ^ 1, data.byte(zero));
  if(slot) slot->writeIO(size, address, data);
}

auto Cartridge::writeIO(uint24 address, uint8 data) -> void {
  if(address == 0xa130f1) ramEnable = data.bit(0), ramWritable = data.bit(1);
  if(address == 0xa130f3) romBank[1] = data;
  if(address == 0xa130f5) romBank[2] = data;
  if(address == 0xa130f7) romBank[3] = data;
  if(address == 0xa130f9) romBank[4] = data;
  if(address == 0xa130fb) romBank[5] = data;
  if(address == 0xa130fd) romBank[6] = data;
  if(address == 0xa130ff) romBank[7] = data;
}

//

auto Cartridge::readLinear(uint1 size, uint22 address, uint16 data) -> uint16 {
  if(ramEnable && ram && address >= 0x200000) {
    if(ramBits == 0xffff) {
      data.byte(1) = ram[address ^ zero];
      data.byte(0) = ram[address ^ size];
    } else {
      data.byte(1) = ram[address >> 1];
      data.byte(0) = ram[address >> 1];
    }
    return data;
  }

  data.byte(1) = rom[address ^ zero];
  data.byte(0) = rom[address ^ size];
  return data;
}

auto Cartridge::writeLinear(uint1 size, uint22 address, uint16 data) -> void {
  //emulating ramWritable will break commercial software:
  //it does not appear that many (any?) games actually connect $a130f1.d1 to /WE;
  //hence RAM ends up always being writable, and many games fail to set d1=1
  if(ramEnable && ram && address >= 0x200000) {
    if(ramBits == 0xffff) {
      ram[address ^ zero] = data.byte(1);
      ram[address ^ size] = data.byte(0);
    } else {
      ram[address >> 1] = data.byte(ramBits == 0xff00);
    }
    return;
  }
}

//

auto Cartridge::readBanked(uint1 size, uint22 address, uint16 data) -> uint16 {
  uint25 offset = romBank[address.bits(19,21)] << 19 | address.bits(0,18);
  data.byte(1) = rom[offset ^ zero];
  data.byte(0) = rom[offset ^ size];
  return data;
}

auto Cartridge::writeBanked(uint1 size, uint22 address, uint16 data) -> void {
}

//

auto Cartridge::readLockOn(uint1 size, uint22 address, uint16 data) -> uint16 {
  if(address < 0x200000) {
    data.byte(1) = rom[address ^ zero];
    data.byte(0) = rom[address ^ size];
    return data;
  }

  if(ramEnable && address >= 0x300000) {
    data.byte(1) = patch[address ^ zero];
    data.byte(0) = patch[address ^ size];
    return data;
  }

  if(slot) return data = slot->read(size, address, data);

  return data;
}

auto Cartridge::writeLockOn(uint1 size, uint22 address, uint16 data) -> void {
  if(slot) return slot->write(size, address, data);
}

//

auto Cartridge::readGameGenie(uint1 size, uint22 address, uint16 data) -> uint16 {
  if(gameGenie.enable) {
    for(auto& code : gameGenie.codes) {
      if(code.enable && code.address == address) {
        data.byte(1) = code.data.byte(zero);
        data.byte(0) = code.data.byte(size);
        return data;
      }
    }
    if(slot) return slot->read(size, address, data);
  }

  data.byte(1) = rom[address ^ zero];
  data.byte(0) = rom[address ^ size];
  return data;
}

auto Cartridge::writeGameGenie(uint1 size, uint22 address, uint16 data) -> void {
  if(gameGenie.enable) {
    if(slot) return slot->write(size, address, data);
  }

  if(address == 0x02 && data == 0x0001) {
    gameGenie.enable = true;
  }

  if(address >= 0x04 && address <= 0x20 && !address.bit(0)) {
    address = address - 0x04 >> 1;
    auto& code = gameGenie.codes[address / 3];
    if(address % 3 == 0) code.address.bits(16,23) = data.byte(0);
    if(address % 3 == 1) code.address.bits( 0,15) = data;
    if(address % 3 == 2) code.data = data, code.enable = true;
  }
}

}
