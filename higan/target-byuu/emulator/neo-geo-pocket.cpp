#include <ngp/interface/interface.hpp>

struct NeoGeoPocket : Emulator {
  NeoGeoPocket();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct NeoGeoPocketColor : Emulator {
  NeoGeoPocketColor();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

NeoGeoPocket::NeoGeoPocket() {
  interface = new higan::NeoGeoPocket::NeoGeoPocketInterface;
  name = "Neo Geo Pocket";
  extensions = {"ngp"};

  firmware.append({"BIOS", "World", "0293555b21c4fac516d25199df7809b26beeae150e1d4504a050db32264a6ad7"});
}

auto NeoGeoPocket::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto node = root->find<higan::Node::Boolean>("Fast Boot")) {
    node->setValue(true);
  }
}

auto NeoGeoPocket::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "bios.rom") {
    if(!file::exists(firmware[0].location)) {
      MessageDialog().setText(
        "In order to run Neo Geo Pocket games, a BIOS is required.\n"
        "Please select the NGP BIOS first. This will only need to be done once.\n"
        "Note: the BIOS file must be uncompressed!"
      ).setAlignment(presentation).information();
      BrowserDialog dialog;
      dialog.setTitle("Select Neo Geo Pocket BIOS");
      dialog.setPath(Path::desktop());
      dialog.setAlignment(presentation);
      string bios = program.openFile(dialog);
      if(file::exists(bios)) {
        auto sha256 = file::sha256(bios);
        if(file::sha256(bios) == firmware[0].sha256) {
          firmware[0].location = bios;
          firmwareSettings.refresh();
        } else {
          MessageDialog().setText(
            "Sorry, this does not appear to be the correct BIOS file. Please try again.\n"
            "Note: the BIOS file must be uncompressed!"
          ).setAlignment(presentation).warning();
        }
      }
    }
    if(auto result = vfs::fs::file::open(firmware[0].location, mode)) return result;
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=Flash)/size"].natural();

  if(name == "program.flash") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  return {};
}

auto NeoGeoPocket::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "A"     ) mapping = virtualPad.a;
  if(name == "B"     ) mapping = virtualPad.b;
  if(name == "Option") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

NeoGeoPocketColor::NeoGeoPocketColor() {
  interface = new higan::NeoGeoPocket::NeoGeoPocketColorInterface;
  name = "Neo Geo Pocket Color";
  extensions = {"ngpc", "ngc"};

  firmware.append({"BIOS", "World", "8fb845a2f71514cec20728e2f0fecfade69444f8d50898b92c2259f1ba63e10d"});
}

auto NeoGeoPocketColor::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto node = root->find<higan::Node::Boolean>("Fast Boot")) {
    node->setValue(true);
  }
}

auto NeoGeoPocketColor::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "bios.rom") {
    if(!file::exists(firmware[0].location)) {
      MessageDialog().setText(
        "In order to run Neo Geo Pocket Color games, a BIOS is required.\n"
        "Please select the NGPC BIOS first. This will only need to be done once.\n"
        "Note: the BIOS file must be uncompressed!"
      ).setAlignment(presentation).information();
      BrowserDialog dialog;
      dialog.setTitle("Select Neo Geo Pocket Color BIOS");
      dialog.setPath(Path::desktop());
      dialog.setAlignment(presentation);
      string bios = program.openFile(dialog);
      if(file::exists(bios)) {
        auto sha256 = file::sha256(bios);
        if(file::sha256(bios) == firmware[0].sha256) {
          firmware[0].location = bios;
          firmwareSettings.refresh();
        } else {
          MessageDialog().setText(
            "Sorry, this does not appear to be the correct BIOS file. Please try again.\n"
            "Note: the BIOS file must be uncompressed!"
          ).setAlignment(presentation).warning();
        }
      }
    }
    if(auto result = vfs::fs::file::open(firmware[0].location, mode)) return result;
    return {};
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=Flash)/size"].natural();

  if(name == "program.flash" && mode == vfs::file::mode::read) {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "program.flash" && mode == vfs::file::mode::write) {
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto NeoGeoPocketColor::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "A"     ) mapping = virtualPad.a;
  if(name == "B"     ) mapping = virtualPad.b;
  if(name == "Option") mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
