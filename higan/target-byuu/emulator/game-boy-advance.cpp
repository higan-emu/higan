#include <gba/interface/interface.hpp>

struct GameBoyAdvance : Emulator {
  GameBoyAdvance();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

GameBoyAdvance::GameBoyAdvance() {
  interface = new higan::GameBoyAdvance::GameBoyAdvanceInterface;
  name = "Game Boy Advance";
  extensions = {"gba"};

  firmware.append({"BIOS", "World", "fd2547724b505f487e6dcb29ec2ecff3af35a841a77ab2e85fd87350abd36570"});
}

auto GameBoyAdvance::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }
}

auto GameBoyAdvance::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "bios.rom") {
    if(!file::exists(firmware[0].location)) {
      MessageDialog().setText(
        "In order to run Game Boy Advance games, a BIOS is required.\n"
        "Please select the GBA BIOS first. This will only need to be done once.\n"
        "Note: the BIOS file must be uncompressed!"
      ).setAlignment(presentation).information();
      BrowserDialog dialog;
      dialog.setTitle("Select Game Boy Advance BIOS");
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
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto saveRAMVolatile = (bool)document["game/board/memory(Content=Save)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" || name == "save.eeprom" || name == "save.flash") {
    if(saveRAMVolatile) return {};
    string location = {Location::notsuffix(game.location), ".sav"};
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto GameBoyAdvance::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "B"     ) mapping = virtualPad.a;
  if(name == "A"     ) mapping = virtualPad.b;
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
