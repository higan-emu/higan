#include <md/interface/interface.hpp>

struct MegaDrive : Emulator {
  MegaDrive();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct MegaCD : Emulator {
  MegaCD();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

MegaDrive::MegaDrive() {
  interface = new higan::MegaDrive::MegaDriveInterface;
  name = "Genesis";
  extensions = {"md", "smd", "gen"};
}

auto MegaDrive::load() -> void {
  if(auto region = root->find<higan::Node::String>("Region")) {
    region->setValue("NTSC-U → NTSC-J → PAL");
  }

  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Fighting Pad");
    port->connect(peripheral);
  }
}

auto MegaDrive::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

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

auto MegaDrive::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"   ) mapping = virtualPad.up;
  if(name == "Down" ) mapping = virtualPad.down;
  if(name == "Left" ) mapping = virtualPad.left;
  if(name == "Right") mapping = virtualPad.right;
  if(name == "A"    ) mapping = virtualPad.x;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "C"    ) mapping = virtualPad.b;
  if(name == "X"    ) mapping = virtualPad.y;
  if(name == "Y"    ) mapping = virtualPad.l;
  if(name == "Z"    ) mapping = virtualPad.r;
  if(name == "Mode" ) mapping = virtualPad.select;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

MegaCD::MegaCD() {
  interface = new higan::MegaDrive::MegaDriveInterface;
  name = "Sega CD";
  extensions = {"bin"};

  firmware.append({"BIOS", "US"});
  firmware.append({"BIOS", "Japan"});
  firmware.append({"BIOS", "Europe"});
}

auto MegaCD::load() -> void {
  bios.location = firmware[0].location;
  bios.image = file::read(bios.location);
  for(auto& media : icarus::media) {
    if(media->name() != "Mega Drive") continue;
    if(auto cartridge = media.cast<icarus::Cartridge>()) {
      bios.manifest = cartridge->manifest(bios.image, bios.location);
    }
  }

  if(auto region = root->find<higan::Node::String>("Region")) {
    region->setValue("NTSC-U → NTSC-J → PAL");
  }

  if(auto port = root->find<higan::Node::Port>("Expansion Port")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    auto peripheral = port->allocate();
    peripheral->setName("Fighting Pad");
    port->connect(peripheral);
  }
}

auto MegaCD::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") {
    return vfs::memory::file::open(bios.manifest.data<uint8_t>(), bios.manifest.size());
  }

  if(name == "program.rom") {
    return vfs::memory::file::open(bios.image.data(), bios.image.size());
  }

  if(name == "backup.ram") {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto MegaCD::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"   ) mapping = virtualPad.up;
  if(name == "Down" ) mapping = virtualPad.down;
  if(name == "Left" ) mapping = virtualPad.left;
  if(name == "Right") mapping = virtualPad.right;
  if(name == "A"    ) mapping = virtualPad.x;
  if(name == "B"    ) mapping = virtualPad.a;
  if(name == "C"    ) mapping = virtualPad.b;
  if(name == "X"    ) mapping = virtualPad.y;
  if(name == "Y"    ) mapping = virtualPad.l;
  if(name == "Z"    ) mapping = virtualPad.r;
  if(name == "Mode" ) mapping = virtualPad.select;
  if(name == "Start") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
