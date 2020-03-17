#include <pce/interface/interface.hpp>

struct PCEngine : Emulator {
  PCEngine();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct PCEngineCD : Emulator {
  PCEngineCD();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct SuperGrafx : Emulator {
  SuperGrafx();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

PCEngine::PCEngine() {
  interface = new higan::PCEngine::PCEngineInterface;
  name = "PC Engine";
  extensions = {"pce"};
}

auto PCEngine::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }

  return true;
}

auto PCEngine::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto programRAMVolatile = (bool)document["game/board/memory(content=Program,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !programRAMVolatile) {
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto PCEngine::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "II"    ) mapping = virtualPad.a;
  if(name == "I"     ) mapping = virtualPad.b;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Run"   ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

PCEngineCD::PCEngineCD() {
  interface = new higan::PCEngine::PCEngineInterface;
  name = "PC Engine CD";
  extensions = {"bin", "img"};

  firmware.append({"BIOS", "World"});
}

auto PCEngineCD::load() -> bool {
  if(!file::exists(firmware[0].location)) {
    errorFirmwareRequired(firmware[0]);
    return false;
  }

  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }

  return true;
}

auto PCEngineCD::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(node->name() == "PC Engine") {
    if(name == "manifest.bml") {
      for(auto& media : icarus::media) {
        if(media->name() != "PC Engine") continue;
        if(auto cartridge = media.cast<icarus::Cartridge>()) {
          if(auto image = loadFirmware(firmware[0])) {
            vector<uint8_t> bios;
            bios.resize(image->size());
            image->read(bios.data(), bios.size());
            auto manifest = cartridge->manifest(bios, firmware[0].location);
            return vfs::memory::file::open(manifest.data<uint8_t>(), manifest.size());
          }
        }
      }
    }

    if(name == "program.rom") {
      return loadFirmware(firmware[0]);
    }

    if(name == "save.ram") {
      auto location = locate(game.location, ".sav", settings.paths.saves);
      if(auto result = vfs::fs::file::open(location, mode)) return result;
    }
  }

  return {};
}

auto PCEngineCD::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "II"    ) mapping = virtualPad.a;
  if(name == "I"     ) mapping = virtualPad.b;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Run"   ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

SuperGrafx::SuperGrafx() {
  interface = new higan::PCEngine::SuperGrafxInterface;
  name = "SuperGrafx";
  extensions = {"sgx"};
}

auto SuperGrafx::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }

  return true;
}

auto SuperGrafx::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();
  auto programRAMVolatile = (bool)document["game/board/memory(content=Program,type=RAM)/volatile"];

  if(name == "program.rom") {
    return vfs::memory::file::open(game.image.data(), programROMSize);
  }

  if(name == "save.ram" && !programRAMVolatile) {
    auto location = locate(game.location, ".sav", settings.paths.saves);
    if(auto result = vfs::fs::file::open(location, mode)) return result;
  }

  return {};
}

auto SuperGrafx::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "Up"    ) mapping = virtualPad.up;
  if(name == "Down"  ) mapping = virtualPad.down;
  if(name == "Left"  ) mapping = virtualPad.left;
  if(name == "Right" ) mapping = virtualPad.right;
  if(name == "II"    ) mapping = virtualPad.a;
  if(name == "I"     ) mapping = virtualPad.b;
  if(name == "Select") mapping = virtualPad.select;
  if(name == "Run"   ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
