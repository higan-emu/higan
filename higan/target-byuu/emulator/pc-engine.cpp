#include <pce/interface/interface.hpp>

struct PCEngine : Emulator {
  PCEngine();
  auto load() -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file>;
  auto input(higan::Node::Input) -> void override;
};

PCEngine::PCEngine() {
  interface = new higan::PCEngine::PCEngineInterface;
  name = "PC Engine";
  abbreviation = "PCE";
  extensions = {"pce"};
}

auto PCEngine::load() -> void {
  if(auto port = root->find<higan::Node::Port>("Cartridge Slot")) {
    auto peripheral = port->allocate();
    port->connect(peripheral);
  }

  if(auto port = root->find<higan::Node::Port>("Controller Port")) {
    auto peripheral = port->allocate();
    peripheral->setName("Gamepad");
    port->connect(peripheral);
  }
}

auto PCEngine::open(higan::Node::Object node, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> {
  if(name == "manifest.bml") {
    for(auto& media : icarus::media) {
      if(media->name() != "PC Engine") continue;
      if(auto cartridge = media.cast<icarus::Cartridge>()) {
        game.manifest = cartridge->manifest(game.data, game.name);
        return vfs::memory::file::open(game.manifest.data<uint8_t>(), game.manifest.size());
      }
    }
    return {};
  }

  auto document = BML::unserialize(game.manifest);

  if(name == "program.rom") {
    return vfs::memory::file::open(game.data.data(), game.data.size());
  }

  if(name == "save.ram") {
    string location = {Location::notsuffix(game.name), ".sav"};
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
