#include <pce/pce.hpp>

namespace higan::PCEngine {

Model model;
Settings settings;
#include "pc-engine.cpp"
#include "supergrafx.cpp"

auto AbstractInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 9;
  display.width  = 280;
  display.height = 240;
  display.internalWidth  = 1120;
  display.internalHeight =  240;
  display.aspectCorrection = 8.0 / 7.0;
  return display;
}

auto AbstractInterface::color(uint32 color) -> uint64 {
  uint3 B = color.bits(0,2);
  uint3 R = color.bits(3,5);
  uint3 G = color.bits(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
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
  {ID::Port::Controller, "Controller"}};
}

auto AbstractInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller) return {
    {ID::Device::Gamepad, "Gamepad"}
  };

  return {};
}

auto AbstractInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "II"    },
    {Type::Button,  "I"     },
    {Type::Control, "Select"},
    {Type::Control, "Run"   }
  };

  return {};
}

auto AbstractInterface::connected(uint port) -> uint {
  if(port == ID::Port::Controller) return settings.controllerPort;
  return 0;
}

auto AbstractInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller) controllerPort.connect(settings.controllerPort = device);
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
  return false;
}

auto AbstractInterface::get(const string& name) -> any {
  return {};
}

auto AbstractInterface::set(const string& name, const any& value) -> bool {
  return false;
}

}
