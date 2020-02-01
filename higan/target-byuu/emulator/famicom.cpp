#include <fc/interface/interface.hpp>

namespace Emulator {

Famicom::Famicom() {
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

auto Famicom::load() -> void {
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

auto Famicom::load(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
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

}
