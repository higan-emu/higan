#include <fc/interface/interface.hpp>

struct Famicom : Emulator {
  Famicom();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct FamicomDiskSystem : Emulator {
  FamicomDiskSystem();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
  auto notify(const string& message) -> void override;

  vector<uint8_t> diskSide[4];
};

Famicom::Famicom() {
  interface = new higan::Famicom::FamicomInterface;
  name = "Nintendo";
  extensions = {"nes"};
}

auto Famicom::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }
}

auto Famicom::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  auto document = BML::unserialize(game.manifest);
  auto iNESROMSize = document["game/board/memory(content=iNES,type=ROM)/size"].natural();
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto characterROMSize = document["game/board/memory(content=Character,type=ROM)/size"].natural();
  auto programRAMSize = (bool)document["game/board/memory(content=Program,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data() + iNESROMSize, programROMSize);
  }

  if(name == "character.rom") {
    return vfs::memory::file::open(game.image.data() + iNESROMSize + programROMSize, characterROMSize);
  }

  if(name == "save.ram" && !programRAMSize) {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto Famicom::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"        ) mapping = virtualPad.up;
  if(name == "Down"      ) mapping = virtualPad.down;
  if(name == "Left"      ) mapping = virtualPad.left;
  if(name == "Right"     ) mapping = virtualPad.right;
  if(name == "B"         ) mapping = virtualPad.a;
  if(name == "A"         ) mapping = virtualPad.b;
  if(name == "Select"    ) mapping = virtualPad.select;
  if(name == "Start"     ) mapping = virtualPad.start;
  if(name == "Microphone") mapping = virtualPad.x;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
       button->setValue(value);
    }
  }
}

FamicomDiskSystem::FamicomDiskSystem() {
  interface = new higan::Famicom::FamicomInterface;
  name = "Famicom Disk System";
  extensions = {"fds"};

  firmware.append({"BIOS", "Japan"});
}

auto FamicomDiskSystem::load() -> void {
  bios.location = firmware[0].location;
  bios.image = file::read(bios.location);
  for(auto& media : icarus::media) {
    if(media->name() != "Famicom") continue;
    if(auto cartridge = media.cast<icarus::Cartridge>()) {
      bios.manifest = cartridge->manifest(bios.image, bios.location);
    }
  }

  for(auto& media : icarus::media) {
    if(media->name() != "Famicom Disk") continue;
    if(auto famicomDisk = media.cast<icarus::FamicomDisk>()) {
      if(game.image.size() % 65500 == 16) {
        //iNES and fwNES headers are unnecessary
        memory::move(&game.image[0], &game.image[16], game.image.size() - 16);
        game.image.resize(game.image.size() - 16);
      }
      array_view<uint8_t> view = game.image;
      uint index = 0;
      while(auto output = famicomDisk->transform(view)) {
        diskSide[index++] = output;
        view += 65500;
        if(index >= 4) break;
      }
    }
  }

  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->scan<higan::Node::Port>("Disk Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto node = root->scan<higan::Node::String>("State")) {
    node->setValue("Disk 1: Side A");
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }
}

auto FamicomDiskSystem::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(node->name() == "Famicom") {
    if(name == "manifest.bml") {
      return vfs::memory::file::open(bios.manifest.data<uint8_t>(), bios.manifest.size());
    }

    if(name == "program.rom") {
      return vfs::memory::file::open(bios.image.data(), bios.image.size());
    }
  }

  if(node->name() == "Famicom Disk") {
    if(name == "manifest.bml") {
      for(auto& media : icarus::media) {
        if(media->name() != "Famicom Disk") continue;
        if(auto floppyDisk = media.cast<icarus::FloppyDisk>()) {
          game.manifest = floppyDisk->manifest(game.image, game.location);
        }
      }
      return vfs::memory::file::open(game.manifest.data<uint8_t>(), game.manifest.size());
    }

    if(name == "disk1.sideA") {
      string location = {Location::notsuffix(game.location), ".1A.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
      if(mode == vfs::file::mode::read) return vfs::memory::file::open(diskSide[0].data(), diskSide[0].size());
    }

    if(name == "disk1.sideB") {
      string location = {Location::notsuffix(game.location), ".1B.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
      if(mode == vfs::file::mode::read) return vfs::memory::file::open(diskSide[1].data(), diskSide[1].size());
    }

    if(name == "disk2.sideA") {
      string location = {Location::notsuffix(game.location), ".2A.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
      if(mode == vfs::file::mode::read) return vfs::memory::file::open(diskSide[2].data(), diskSide[2].size());
    }

    if(name == "disk2.sideB") {
      string location = {Location::notsuffix(game.location), ".2B.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
      if(mode == vfs::file::mode::read) return vfs::memory::file::open(diskSide[3].data(), diskSide[3].size());
    }
  }

  return {};
}

auto FamicomDiskSystem::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"        ) mapping = virtualPad.up;
  if(name == "Down"      ) mapping = virtualPad.down;
  if(name == "Left"      ) mapping = virtualPad.left;
  if(name == "Right"     ) mapping = virtualPad.right;
  if(name == "B"         ) mapping = virtualPad.a;
  if(name == "A"         ) mapping = virtualPad.b;
  if(name == "Select"    ) mapping = virtualPad.select;
  if(name == "Start"     ) mapping = virtualPad.start;
  if(name == "Microphone") mapping = virtualPad.x;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
       button->setValue(value);
    }
  }
}

auto FamicomDiskSystem::notify(const string& message) -> void {
  if(auto node = root->scan<higan::Node::String>("State")) {
    node->setValue(message);
  }
}
