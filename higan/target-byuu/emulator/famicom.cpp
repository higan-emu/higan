#include <fc/interface/interface.hpp>

FamicomEmulator::FamicomEmulator() {
  interface = new higan::Famicom::FamicomInterface;
  name = "Nintendo";
  abbreviation = "NES";
  extensions = {"nes"};

  { Port port;
    port.name = "Controller Port 1";
    ports.append(port);
  }

  { Port port;
    port.name = "Controller Port 2";
    ports.append(port);
  }
}

auto FamicomEmulator::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    peripheral->setAttribute("id", "rom");
    peripheral->setAttribute("location", game.name);
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }
}

auto FamicomEmulator::load(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
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

  return {};
}

auto FamicomEmulator::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up") mapping = virtualPad.up;
  if(name == "Down") mapping = virtualPad.down;
  if(name == "Left") mapping = virtualPad.left;
  if(name == "Right") mapping = virtualPad.right;
  if(name == "B") mapping = virtualPad.a;
  if(name == "A") mapping = virtualPad.b;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
       button->setValue(value);
    }
  }
}
