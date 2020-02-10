#include <ws/interface/interface.hpp>

struct WonderSwan : Emulator {
  WonderSwan();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

WonderSwan::WonderSwan() {
  interface = new higan::WonderSwan::WonderSwanInterface;
  name = "WonderSwan";
  abbreviation = "WS";
  extensions = {"ws"};
}

auto WonderSwan::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }
}

auto WonderSwan::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "boot.rom") {
    return vfs::memory::file::open(Resource::WonderSwan::Boot, sizeof Resource::WonderSwan::Boot);
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(Content=Save,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !saveRAMVolatile) {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto WonderSwan::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Y1"   ) mapping = virtualPad.c;
  if(name == "Y2"   ) mapping = virtualPad.x;
  if(name == "Y3"   ) mapping = virtualPad.y;
  if(name == "Y4"   ) mapping = virtualPad.z;
  if(name == "X1"   ) mapping = virtualPad.up;
  if(name == "X2"   ) mapping = virtualPad.right;
  if(name == "X3"   ) mapping = virtualPad.down;
  if(name == "X4"   ) mapping = virtualPad.left;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "A"    ) mapping = virtualPad.b;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

struct WonderSwanColor : Emulator {
  WonderSwanColor();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

WonderSwanColor::WonderSwanColor() {
  interface = new higan::WonderSwan::WonderSwanColorInterface;
  name = "WonderSwan Color";
  abbreviation = "WSC";
  extensions = {"wsc"};
}

auto WonderSwanColor::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }
}

auto WonderSwanColor::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "boot.rom") {
    return vfs::memory::file::open(Resource::WonderSwanColor::Boot, sizeof Resource::WonderSwanColor::Boot);
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(Content=Save,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !saveRAMVolatile) {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto WonderSwanColor::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Y1"   ) mapping = virtualPad.c;
  if(name == "Y2"   ) mapping = virtualPad.x;
  if(name == "Y3"   ) mapping = virtualPad.y;
  if(name == "Y4"   ) mapping = virtualPad.z;
  if(name == "X1"   ) mapping = virtualPad.up;
  if(name == "X2"   ) mapping = virtualPad.right;
  if(name == "X3"   ) mapping = virtualPad.down;
  if(name == "X4"   ) mapping = virtualPad.left;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "A"    ) mapping = virtualPad.b;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
