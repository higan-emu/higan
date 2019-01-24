#include <md/md.hpp>

namespace higan::MegaDrive {

Settings settings;

auto MegaDriveInterface::information() -> Information {
  Information information;
  information.manufacturer = "Sega";
  information.name         = "Mega Drive";
  information.extension    = "md";
  information.resettable   = true;
  return information;
}

auto MegaDriveInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 3 * (1 << 9);
  display.width  = 320;
  display.height = 240;
  display.internalWidth  = 1280;
  display.internalHeight =  480;
  display.aspectCorrection = 1.0;
  return display;
}

auto MegaDriveInterface::color(uint32 color) -> uint64 {
  uint R = color.bits(0, 2);
  uint G = color.bits(3, 5);
  uint B = color.bits(6, 8);
  uint M = color.bits(9,10);

  uint lookup[3][8] = {
    {  0,  29,  52,  70,  87, 101, 116, 130},  //shadow
    {  0,  52,  87, 116, 144, 172, 206, 255},  //normal
    {130, 144, 158, 172, 187, 206, 228, 255},  //highlight
  };

  uint64 r = image::normalize(lookup[M][R], 8, 16);
  uint64 g = image::normalize(lookup[M][G], 8, 16);
  uint64 b = image::normalize(lookup[M][B], 8, 16);

  return r << 32 | g << 16 | b << 0;
}

auto MegaDriveInterface::loaded() -> bool {
  return system.loaded();
}

auto MegaDriveInterface::hashes() -> vector<string> {
  return cartridge.hashes();
}

auto MegaDriveInterface::manifests() -> vector<string> {
  return cartridge.manifests();
}

auto MegaDriveInterface::titles() -> vector<string> {
  return cartridge.titles();
}

auto MegaDriveInterface::load() -> bool {
  return system.load(this);
}

auto MegaDriveInterface::save() -> void {
  system.save();
}

auto MegaDriveInterface::unload() -> void {
  save();
  system.unload();
}

auto MegaDriveInterface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"},
  {ID::Port::Extension,   "Extension Port"   }};
}

auto MegaDriveInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,        "None"        },
    {ID::Device::ControlPad,  "Control Pad" },
    {ID::Device::FightingPad, "Fighting Pad"}
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,        "None"        },
    {ID::Device::ControlPad,  "Control Pad" },
    {ID::Device::FightingPad, "Fighting Pad"}
  };

  if(port == ID::Port::Extension) return {
    {ID::Device::None, "None"}
  };

  return {};
}

auto MegaDriveInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::None) return {
  };

  if(device == ID::Device::ControlPad) return {
    {Type::Hat,     "Up"   },
    {Type::Hat,     "Down" },
    {Type::Hat,     "Left" },
    {Type::Hat,     "Right"},
    {Type::Button,  "A"    },
    {Type::Button,  "B"    },
    {Type::Button,  "C"    },
    {Type::Control, "Start"}
  };

  if(device == ID::Device::FightingPad) return {
    {Type::Hat,     "Up"   },
    {Type::Hat,     "Down" },
    {Type::Hat,     "Left" },
    {Type::Hat,     "Right"},
    {Type::Button,  "A"    },
    {Type::Button,  "B"    },
    {Type::Button,  "C"    },
    {Type::Button,  "X"    },
    {Type::Button,  "Y"    },
    {Type::Button,  "Z"    },
    {Type::Control, "Mode" },
    {Type::Control, "Start"}
  };

  return {};
}

auto MegaDriveInterface::connected(uint port) -> uint {
  if(port == ID::Port::Controller1) return settings.controllerPort1;
  if(port == ID::Port::Controller2) return settings.controllerPort2;
  if(port == ID::Port::Extension) return settings.extensionPort;
  return 0;
}

auto MegaDriveInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(settings.controllerPort1 = device);
  if(port == ID::Port::Controller2) controllerPort2.connect(settings.controllerPort2 = device);
  if(port == ID::Port::Extension) extensionPort.connect(settings.extensionPort = device);
}

auto MegaDriveInterface::power() -> void {
  system.power(/* reset = */ false);
}

auto MegaDriveInterface::reset() -> void {
  system.power(/* reset = */ true);
}

auto MegaDriveInterface::run() -> void {
  system.run();
}

auto MegaDriveInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto MegaDriveInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto MegaDriveInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

auto MegaDriveInterface::cap(const string& name) -> bool {
  return false;
}

auto MegaDriveInterface::get(const string& name) -> any {
  return {};
}

auto MegaDriveInterface::set(const string& name, const any& value) -> bool {
  return false;
}

}
