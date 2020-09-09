#include <n64/interface/interface.hpp>

struct Nintendo64 : Emulator {
  Nintendo64();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

struct Nintendo64DD : Emulator {
  Nintendo64DD();
  auto load() -> bool override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto input(higan::Node::Input) -> void override;
};

Nintendo64::Nintendo64() {
  interface = new higan::Nintendo64::Nintendo64Interface;
  medium = icarus::medium("Nintendo 64");
  manufacturer = "Nintendo";
  name = "Nintendo 64";
}

auto Nintendo64::load() -> bool {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    port->allocate("Gamepad");
    port->connect();
  }

  return true;
}

auto Nintendo64::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") return Emulator::manifest();

  if(name == "pif.rom") {
    return vfs::memory::open(Resource::Nintendo64::PIF::ROM, sizeof Resource::Nintendo64::PIF::ROM);
  }

  if(name == "pif.ntsc.rom") {
    return vfs::memory::open(Resource::Nintendo64::PIF::NTSC, sizeof Resource::Nintendo64::PIF::NTSC);
  }

  if(name == "pif.pal.rom") {
    return vfs::memory::open(Resource::Nintendo64::PIF::PAL, sizeof Resource::Nintendo64::PIF::PAL);
  }

  auto document = BML::unserialize(game.manifest);
  auto programROMSize = document["game/board/memory(content=Program,type=ROM)/size"].natural();

  if(name == "program.rom") {
    return vfs::memory::open(game.image.data(), programROMSize);
  }

  return {};
}

auto Nintendo64::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "X-axis" ) mapping = virtualPad.lStickAxisX;
  if(name == "Y-axis" ) mapping = virtualPad.lStickAxisY;
  if(name == "Up"     ) mapping = virtualPad.up;
  if(name == "Down"   ) mapping = virtualPad.down;
  if(name == "Left"   ) mapping = virtualPad.left;
  if(name == "Right"  ) mapping = virtualPad.right;
  if(name == "B"      ) mapping = virtualPad.x;
  if(name == "A"      ) mapping = virtualPad.a;
  if(name == "C-Up"   ) mapping = virtualPad.rStickUp;
  if(name == "C-Down" ) mapping = virtualPad.rStickDown;
  if(name == "C-Left" ) mapping = virtualPad.rStickLeft;
  if(name == "C-Right") mapping = virtualPad.rStickRight;
  if(name == "L"      ) mapping = virtualPad.lb;
  if(name == "R"      ) mapping = virtualPad.rb;
  if(name == "Z"      ) mapping = virtualPad.lt;
  if(name == "Start"  ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto axis = node->cast<higan::Node::Axis>()) {
      axis->setValue(value);
    }
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}

Nintendo64DD::Nintendo64DD() {
  interface = new higan::Nintendo64::Nintendo64Interface;
  medium = icarus::medium("Nintendo 64DD");
  manufacturer = "Nintendo";
  name = "Nintendo 64DD";

  firmware.append({"BIOS", "Japan"});
}

auto Nintendo64DD::load() -> bool {
  if(!file::exists(firmware[0].location)) {
    errorFirmwareRequired(firmware[0]);
    return false;
  }

  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    port->allocate();
    port->connect();
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port 1")) {
    port->allocate("Gamepad");
    port->connect();
  }

  return true;
}

auto Nintendo64DD::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(node->name() == "Disk Drive") {
    if(name == "program.rom") {
      return loadFirmware(firmware[0]);
    }
  }

  if(node->name() == "Nintendo 64DD") {
  }

  return {};
}

auto Nintendo64DD::input(higan::Node::Input node) -> void {
  auto name = node->name();
  maybe<InputMapping&> mapping;
  if(name == "X-axis" ) mapping = virtualPad.lStickAxisX;
  if(name == "Y-axis" ) mapping = virtualPad.lStickAxisY;
  if(name == "Up"     ) mapping = virtualPad.up;
  if(name == "Down"   ) mapping = virtualPad.down;
  if(name == "Left"   ) mapping = virtualPad.left;
  if(name == "Right"  ) mapping = virtualPad.right;
  if(name == "B"      ) mapping = virtualPad.x;
  if(name == "A"      ) mapping = virtualPad.a;
  if(name == "C-Up"   ) mapping = virtualPad.rStickUp;
  if(name == "C-Down" ) mapping = virtualPad.rStickDown;
  if(name == "C-Left" ) mapping = virtualPad.rStickLeft;
  if(name == "C-Right") mapping = virtualPad.rStickRight;
  if(name == "L"      ) mapping = virtualPad.lb;
  if(name == "R"      ) mapping = virtualPad.rb;
  if(name == "Z"      ) mapping = virtualPad.lt;
  if(name == "Start"  ) mapping = virtualPad.start;

  if(mapping) {
    auto value = mapping->value();
    if(auto axis = node->cast<higan::Node::Axis>()) {
      axis->setValue(value);
    }
    if(auto button = node->cast<higan::Node::Button>()) {
      button->setValue(value);
    }
  }
}
