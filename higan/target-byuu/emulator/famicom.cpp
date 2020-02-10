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
