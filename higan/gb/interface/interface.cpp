#include <gb/gb.hpp>

namespace higan::GameBoy {

SuperGameBoyInterface* superGameBoy = nullptr;
Settings settings;
#include "game-boy.cpp"
#include "game-boy-color.cpp"

auto AbstractInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = Model::GameBoyColor() ? 1 << 15 : 1 << 2;
  display.width  = 160;
  display.height = 144;
  display.internalWidth  = 160;
  display.internalHeight = 144;
  display.aspectCorrection = 1.0;
  return display;
}

auto AbstractInterface::loaded() -> bool {
  return system.loaded();
}

auto AbstractInterface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto AbstractInterface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto AbstractInterface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto AbstractInterface::save() -> void {
  system.save();
}

auto AbstractInterface::unload() -> void {
  save();
  system.unload();
}

auto AbstractInterface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"},
  {ID::Port::Cartridge, "Cartridge"}};
}

auto AbstractInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::Controls, "Controls"}
  };

  if(port == ID::Port::Cartridge) return {
    {ID::Device::MBC5, "MBC5"},
    {ID::Device::MBC7, "MBC7"}
  };

  return {};
}

auto AbstractInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Controls) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "B"     },
    {Type::Button,  "A"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" }
  };

  if(device == ID::Device::MBC5) return {
    {Type::Rumble, "Rumble"}
  };

  if(device == ID::Device::MBC7) return {
    {Type::Axis, "Accelerometer - X-axis"},
    {Type::Axis, "Accelerometer - Y-axis"}
  };

  return {};
}

auto AbstractInterface::power() -> void {
  system.power();
}

auto AbstractInterface::run() -> void {
  system.run();
}

auto AbstractInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto AbstractInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto AbstractInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

auto AbstractInterface::cap(const string& name) -> bool {
  if(name == "Blur Emulation") return true;
  if(name == "Color Emulation") return true;
  return false;
}

auto AbstractInterface::get(const string& name) -> any {
  if(name == "Blur Emulation") return settings.blurEmulation;
  if(name == "Color Emulation") return settings.colorEmulation;
  return {};
}

auto AbstractInterface::set(const string& name, const any& value) -> bool {
  if(name == "Blur Emulation" && value.is<bool>()) {
    settings.blurEmulation = value.get<bool>();
    if(Model::SuperGameBoy()) return true;
    video.setEffect(Video::Effect::InterframeBlending, settings.blurEmulation);
    return true;
  }

  if(name == "Color Emulation" && value.is<bool>()) {
    settings.colorEmulation = value.get<bool>();
    if(Model::SuperGameBoy()) return true;
    video.setPalette();
    return true;
  }

  return false;
}

}
