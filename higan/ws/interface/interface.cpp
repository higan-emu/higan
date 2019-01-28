#include <ws/ws.hpp>

namespace higan::WonderSwan {

Options option;
Properties propertyWonderSwan;
Properties propertyWonderSwanColor;
Properties propertyPocketChallengeV2;
#include "wonderswan.cpp"
#include "wonderswan-color.cpp"
#include "pocket-challenge-v2.cpp"

auto AbstractInterface::display() -> Display {
  Display display;
  display.type = Display::Type::LCD;
  display.colors = 1 << 12;
  display.width  = 224;
  display.height = 144;
  display.internalWidth  = 224;
  display.internalHeight = 144;
  display.aspectCorrection = 1.0;
  if(option.video.rotateLeft()) {
    swap(display.width, display.height);
    swap(display.internalWidth, display.internalHeight);
  }
  return display;
}

//todo: the WonderSwan and PocketChallengeV2 interfaces should be generating grayscale shades
//instead, the PPU is currently selecting grayscale shades from the WonderSwanColor palette

auto AbstractInterface::color(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(option.video.colorEmulation()) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
}

auto AbstractInterface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"}};
}

auto AbstractInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::Controls, "Controls"}
  };

  return {};
}

auto AbstractInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Controls) return {
    {Type::Hat,     "Y1"   },
    {Type::Hat,     "Y2"   },
    {Type::Hat,     "Y3"   },
    {Type::Hat,     "Y4"   },
    {Type::Hat,     "X1"   },
    {Type::Hat,     "X2"   },
    {Type::Hat,     "X3"   },
    {Type::Hat,     "X4"   },
    {Type::Button,  "B"    },
    {Type::Button,  "A"    },
    {Type::Control, "Start"}
  };

  return {};
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

auto AbstractInterface::options() -> Settings& {
  return option;
}

}
