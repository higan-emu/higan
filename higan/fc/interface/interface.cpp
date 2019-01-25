#include <fc/fc.hpp>

namespace higan::Famicom {

Options option;
Properties property;

auto FamicomInterface::information() -> Information {
  Information information;
  information.manufacturer = "Nintendo";
  information.name         = "Famicom";
  information.extension    = "fc";
  information.resettable   = true;
  return information;
}

auto FamicomInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 9;
  display.width  = 256;
  display.height = 240;
  display.internalWidth  = 256;
  display.internalHeight = 240;
  display.aspectCorrection = 8.0 / 7.0;
  return display;
}

auto FamicomInterface::color(uint32 n) -> uint64 {
  double saturation = 2.0;
  double hue = 0.0;
  double contrast = 1.0;
  double brightness = 1.0;
  double gamma = option.video.colorEmulation() ? 1.8 : 2.2;

  int color = (n & 0x0f), level = color < 0xe ? int(n >> 4 & 3) : 1;

  static const double black = 0.518, white = 1.962, attenuation = 0.746;
  static const double levels[8] = {
    0.350, 0.518, 0.962, 1.550,
    1.094, 1.506, 1.962, 1.962,
  };

  double lo_and_hi[2] = {
    levels[level + 4 * (color == 0x0)],
    levels[level + 4 * (color <  0xd)],
  };

  double y = 0.0, i = 0.0, q = 0.0;
  auto wave = [](int p, int color) { return (color + p + 8) % 12 < 6; };
  for(int p : range(12)) {
    double spot = lo_and_hi[wave(p, color)];

    if(((n & 0x040) && wave(p, 12))
    || ((n & 0x080) && wave(p,  4))
    || ((n & 0x100) && wave(p,  8))
    ) spot *= attenuation;

    double v = (spot - black) / (white - black);

    v = (v - 0.5) * contrast + 0.5;
    v *= brightness / 12.0;

    y += v;
    i += v * cos((Math::Pi / 6.0) * (p + hue));
    q += v * sin((Math::Pi / 6.0) * (p + hue));
  }

  i *= saturation;
  q *= saturation;

  auto gammaAdjust = [=](double f) { return f < 0.0 ? 0.0 : pow(f, 2.2 / gamma); };
  uint64 r = uclamp<16>(65535.0 * gammaAdjust(y +  0.946882 * i +  0.623557 * q));
  uint64 g = uclamp<16>(65535.0 * gammaAdjust(y + -0.274788 * i + -0.635691 * q));
  uint64 b = uclamp<16>(65535.0 * gammaAdjust(y + -1.108545 * i +  1.709007 * q));

  return r << 32 | g << 16 | b << 0;
}

auto FamicomInterface::loaded() -> bool {
  return system.loaded();
}

auto FamicomInterface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto FamicomInterface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto FamicomInterface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto FamicomInterface::load() -> bool {
  return system.load(this);
}

auto FamicomInterface::save() -> void {
  system.save();
}

auto FamicomInterface::unload() -> void {
  save();
  system.unload();
}

auto FamicomInterface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"},
  {ID::Port::Expansion,   "Expansion Port"   }};
}

auto FamicomInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"}
  };

  if(port == ID::Port::Expansion) return {
    {ID::Device::None, "None"}
  };

  return {};
}

auto FamicomInterface::inputs(uint device) -> vector<Input> {
  using Type = Input::Type;

  if(device == ID::Device::None) return {
  };

  if(device == ID::Device::Gamepad) return {
    {Type::Hat,     "Up"    },
    {Type::Hat,     "Down"  },
    {Type::Hat,     "Left"  },
    {Type::Hat,     "Right" },
    {Type::Button,  "B"     },
    {Type::Button,  "A"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" }
  };

  return {};
}

auto FamicomInterface::connected(uint port) -> uint {
  if(port == ID::Port::Controller1) return option.port.controller1.device();
  if(port == ID::Port::Controller2) return option.port.controller2.device();
  if(port == ID::Port::Expansion) return option.port.expansion.device();
  return 0;
}

auto FamicomInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(option.port.controller1.device(device));
  if(port == ID::Port::Controller2) controllerPort2.connect(option.port.controller2.device(device));
}

auto FamicomInterface::power() -> void {
  system.power(/* reset = */ false);
}

auto FamicomInterface::reset() -> void {
  system.power(/* reset = */ true);
}

auto FamicomInterface::run() -> void {
  system.run();
}

auto FamicomInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto FamicomInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto FamicomInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

auto FamicomInterface::options() -> Settings& {
  return option;
}

auto FamicomInterface::properties() -> Settings& {
  return property;
}

}
