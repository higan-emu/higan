#include <gba/gba.hpp>

namespace higan::GameBoyAdvance {

Interface* interface = nullptr;
Options option;
Properties property;

auto GameBoyAdvanceInterface::information() -> Information {
  Information information;
  information.manufacturer = "Nintendo";
  information.name         = "Game Boy Advance";
  information.extension    = "gba";
  return information;
}

auto GameBoyAdvanceInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::LCD;
  display.colors = 1 << 15;
  display.width  = 240;
  display.height = 160;
  display.internalWidth  = 240;
  display.internalHeight = 160;
  display.aspectCorrection = 1.0;
  if(option.video.rotateLeft()) {
    swap(display.width, display.height);
    swap(display.internalWidth, display.internalHeight);
  }
  return display;
}

auto GameBoyAdvanceInterface::color(uint32 color) -> uint64 {
  uint R = color.bits( 0, 4);
  uint G = color.bits( 5, 9);
  uint B = color.bits(10,14);

  uint64 r = image::normalize(R, 5, 16);
  uint64 g = image::normalize(G, 5, 16);
  uint64 b = image::normalize(B, 5, 16);

  if(option.video.colorEmulation()) {
    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
  }

  return r << 32 | g << 16 | b << 0;
}

auto GameBoyAdvanceInterface::slots() -> vector<Slot> {
  return {cartridge.slot};
}

auto GameBoyAdvanceInterface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto GameBoyAdvanceInterface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto GameBoyAdvanceInterface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto GameBoyAdvanceInterface::initialize() -> void {
  interface = this;
  return system.initialize();
}

auto GameBoyAdvanceInterface::load() -> bool {
  return system.load();
}

auto GameBoyAdvanceInterface::load(uint id) -> bool {
  if(id == cartridge.slot.id) return cartridge.load();
  return false;
}

auto GameBoyAdvanceInterface::save() -> void {
  system.save();
}

auto GameBoyAdvanceInterface::unload() -> void {
  save();
  system.unload();
}

auto GameBoyAdvanceInterface::root() -> Node {
  return system.node;
}

auto GameBoyAdvanceInterface::ports() -> vector<Port> { return {
  {ID::Port::Hardware, "Hardware"}};
}

auto GameBoyAdvanceInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Hardware) return {
    {ID::Device::Controls, "Controls"}
  };

  return {};
}

auto GameBoyAdvanceInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::Controls) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "B"     },
    {Type::Button,  "A"     },
    {Type::Trigger, "L"     },
    {Type::Trigger, "R"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" },
    {Type::Rumble,  "Rumble"}
  };

  return {};
}

auto GameBoyAdvanceInterface::power() -> void {
  system.power();
}

auto GameBoyAdvanceInterface::run() -> void {
  system.run();
}

auto GameBoyAdvanceInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto GameBoyAdvanceInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto GameBoyAdvanceInterface::options() -> Settings& {
  return option;
}

auto GameBoyAdvanceInterface::properties() -> Settings& {
  return property;
}

}
