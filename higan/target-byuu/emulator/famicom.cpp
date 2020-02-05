#include <fc/interface/interface.hpp>

struct Famicom : Emulator {
  Famicom();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

Famicom::Famicom() {
  interface = new higan::Famicom::FamicomInterface;
  name = "Nintendo";
  abbreviation = "NES";
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
  if(name == "manifest.bml") {
    for(auto& media : icarus::media) {
      if(media->name() != "Famicom") continue;
      if(auto cartridge = media.cast<icarus::Cartridge>()) {
        game.manifest = cartridge->manifest(game.data, game.name);
        return vfs::memory::file::open(game.manifest.data<uint8_t>(), game.manifest.size());
      }
    }
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto headerSize = document["game/board/memory(content=iNES,type=ROM)/size"].natural();
  auto programSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto characterSize = document["game/board/memory(content=Character,type=ROM)/size"].natural();

  if(name == "program.rom") {
    uint address = headerSize;
    return vfs::memory::file::open(game.data.data() + address, programSize);
  }

  if(name == "character.rom") {
    uint address = headerSize + programSize;
    return vfs::memory::file::open(game.data.data() + address, characterSize);
  }

  if(name == "save.ram") {
    string location = {Location::notsuffix(game.name), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto Famicom::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "B"     ) mapping = virtualPad.a;
  if(name == "A"     ) mapping = virtualPad.b;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Start" ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
       button->setValue(value);
    }
  }
}
