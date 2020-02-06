#include <sfc/interface/interface.hpp>

struct SuperFamicom : Emulator {
  SuperFamicom();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

SuperFamicom::SuperFamicom() {
  interface = new higan::SuperFamicom::SuperFamicomInterface;
  name = "Super Nintendo";
  abbreviation = "SNES";
  extensions = {"sfc", "smc"};
}

auto SuperFamicom::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Port")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }
}

auto SuperFamicom::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "boards.bml") {
    return vfs::memory::file::open(Resource::SuperFamicom::Boards, sizeof Resource::SuperFamicom::Boards);
  }

  if(name == "ipl.rom") {
    return vfs::memory::file::open(Resource::SuperFamicom::IPLROM, sizeof Resource::SuperFamicom::IPLROM);
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto dataROMSize = document["game/board/memory(content=Data,type=ROM)/size"].natural();
  auto expansionROMSize = document["game/board/memory(content=Expansion.type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(content=Save,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "data.rom") {
    return vfs::memory::file::open(game.image.data() + programROMSize, dataROMSize);
  }

  if(name == "expansion.rom") {
    return vfs::memory::file::open(game.image.data() + programROMSize + dataROMSize, expansionROMSize);
  }

  if(name == "save.ram") {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  if(name == "download.ram") {
    string location = {Location::notsuffix(game.location), ".psr"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  if(name == "time.rtc") {
    string location = {Location::notsuffix(game.location), ".rtc"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  if(name.beginsWith("upd7725.")) {
    auto firmwareProgramROMSize = document["game/board/memory(content=Program,type=ROM,architecture=uPD7725)/size"].natural();
    auto firmwareDataROMSize = document["game/board/memory(content=Program,type=ROM,architecture=uPD7725)/size"].natural();

    if(name == "upd7725.program.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize, programROMSize);
    }

    if(name == "upd7725.data.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize + firmwareProgramROMSize, firmwareDataROMSize);
    }
  }

  if(name.beginsWith("upd96050.")) {
    auto firmwareProgramROMSize = document["game/board/memory(content=Program,type=ROM,architecture=uPD96050)/size"].natural();
    auto firmwareDataROMSize = document["game/board/memory(content=Program,type=ROM,architecture=uPD96050)/size"].natural();
    auto firmwareDataRAMVolatile = (bool)document["game/board/memory(content=Program,type=RAM,architecture=uPD96050)/volatile"];

    if(name == "upd96050.program.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize, programROMSize);
    }

    if(name == "upd96050.data.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize + firmwareProgramROMSize, firmwareDataROMSize);
    }

    if(name == "upd96050.data.ram" && !firmwareDataRAMVolatile) {
      string location = {Location::notsuffix(game.location), ".upd96050.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
    }
  }

  if(name.beginsWith("hg51bs169.")) {
    auto firmwareDataROMSize = document["game/board/memory(content=Data,type=ROM,architecture=HG51BS169)/size"].natural();
    auto firmwareDataRAMVolatile = (bool)document["game/board/memory(content=Data,type=RAM,architecture=HG51BS169)/volatile"];

    if(name == "hg51bs169.data.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize, firmwareDataROMSize);
    }

    if(name == "hg51bs169.data.ram" && !firmwareDataRAMVolatile) {
      string location = {Location::notsuffix(game.location), ".hg51bs169.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
    }
  }

  if(name.beginsWith("arm6.")) {
    auto firmwareProgramROMSize = document["game/board/memory(content=Program,type=ROM,architecture=ARM6)/size"].natural();
    auto firmwareDataROMSize = document["game/board/memory(content=Data,type=ROM,architecture=ARM6)/size"].natural();
    auto firmwareDataRAMVolatile = (bool)document["game/board/memory(content=Data,type=RAM,architecture=ARM6)/volatile"];

    if(name == "arm6.program.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize, firmwareProgramROMSize);
    }

    if(name == "arm6.data.rom") {
      return vfs::memory::file::open(game.image.data() + programROMSize + firmwareProgramROMSize, firmwareDataROMSize);
    }

    if(name == "arm6.data.ram" && !firmwareDataRAMVolatile) {
      string location = {Location::notsuffix(game.location), ".arm6.sav"};
      if(auto result = vfs::fs::file::open(location, mode)) return result;
    }
  }

  if(name == "msu1/data.rom") {
    string location = {Location::notsuffix(game.location), ".msu"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  if(name.beginsWith("msu1/track")) {
    string location = {Location::notsuffix(game.location), "-track", string{name}.trimLeft("msu1/track", 1L)};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto SuperFamicom::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "B"     ) mapping = virtualPad.a;
  if(name == "A"     ) mapping = virtualPad.b;
  if(name == "Y"     ) mapping = virtualPad.x;
  if(name == "X"     ) mapping = virtualPad.y;
  if(name == "L"     ) mapping = virtualPad.l;
  if(name == "R"     ) mapping = virtualPad.r;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Start" ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
