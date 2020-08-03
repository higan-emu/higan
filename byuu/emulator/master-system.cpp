#include <ms/interface/interface.hpp>

struct MasterSystem : Emulator {
  MasterSystem();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct GameGear : Emulator {
  GameGear();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

MasterSystem::MasterSystem() {
  interface = new higan::MasterSystem::MasterSystemInterface;
  medium = icarus::medium("Master System");
  manufacturer = "Sega";
  name = "Master System";
}

auto MasterSystem::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    port->allocate("Gamepad");
    port->connect();
  }

  return true;
}

auto MasterSystem::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(Content=Save,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !saveRAMVolatile) {
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  return {};
}

auto MasterSystem::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Pause") mapping = virtualPad.start;
  if(name == "Reset") mapping = nothing;
  if(name == "Up"   ) mapping = virtualPad.up;
  if(name == "Down" ) mapping = virtualPad.down;
  if(name == "Left" ) mapping = virtualPad.left;
  if(name == "Right") mapping = virtualPad.right;
  if(name == "1"    ) mapping = virtualPad.a;
  if(name == "2"    ) mapping = virtualPad.b;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

GameGear::GameGear() {
  interface = new higan::MasterSystem::GameGearInterface;
  medium = icarus::medium("Game Gear");
  manufacturer = "Sega";
  name = "Game Gear";
}

auto GameGear::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  return true;
}

auto GameGear::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(Content=Save,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !saveRAMVolatile) {
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::disk::open(location, mode)) return result;
  }

  return {};
}

auto GameGear::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"   ) mapping = virtualPad.up;
  if(name == "Down" ) mapping = virtualPad.down;
  if(name == "Left" ) mapping = virtualPad.left;
  if(name == "Right") mapping = virtualPad.right;
  if(name == "1"    ) mapping = virtualPad.a;
  if(name == "2"    ) mapping = virtualPad.b;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
