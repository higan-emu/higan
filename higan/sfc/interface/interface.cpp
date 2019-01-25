#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

Options option;
Properties property;

auto SuperFamicomInterface::information() -> Information {
  Information information;
  information.manufacturer = "Nintendo";
  information.name         = "Super Famicom";
  information.extension    = "sfc";
  information.resettable   = true;
  return information;
}

auto SuperFamicomInterface::display() -> Display {
  Display display;
  display.type   = Display::Type::CRT;
  display.colors = 1 << 19;
  display.width  = 256;
  display.height = 240;
  display.internalWidth  = 512;
  display.internalHeight = 480;
  display.aspectCorrection = 8.0 / 7.0;
  return display;
}

auto SuperFamicomInterface::color(uint32 color) -> uint64 {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);
  uint l = color.bits(15,18);

  //luma=0 is not 100% black; but it's much darker than normal linear scaling
  //exact effect seems to be analog; requires > 24-bit color depth to represent accurately
  double L = (1.0 + l) / 16.0 * (l ? 1.0 : 0.25);
  uint64 R = L * image::normalize(r, 5, 16);
  uint64 G = L * image::normalize(g, 5, 16);
  uint64 B = L * image::normalize(b, 5, 16);

  if(option.video.colorEmulation()) {
    static const uint8 gammaRamp[32] = {
      0x00, 0x01, 0x03, 0x06, 0x0a, 0x0f, 0x15, 0x1c,
      0x24, 0x2d, 0x37, 0x42, 0x4e, 0x5b, 0x69, 0x78,
      0x88, 0x90, 0x98, 0xa0, 0xa8, 0xb0, 0xb8, 0xc0,
      0xc8, 0xd0, 0xd8, 0xe0, 0xe8, 0xf0, 0xf8, 0xff,
    };
    R = L * gammaRamp[r] * 0x0101;
    G = L * gammaRamp[g] * 0x0101;
    B = L * gammaRamp[b] * 0x0101;
  }

  return R << 32 | G << 16 | B << 0;
}

auto SuperFamicomInterface::loaded() -> bool {
  return system.loaded();
}

auto SuperFamicomInterface::hashes() -> vector<string> {
  return cartridge.hashes();
}

auto SuperFamicomInterface::manifests() -> vector<string> {
  return cartridge.manifests();
}

auto SuperFamicomInterface::titles() -> vector<string> {
  return cartridge.titles();
}

auto SuperFamicomInterface::load() -> bool {
  return system.load(this);
}

auto SuperFamicomInterface::save() -> void {
  system.save();
}

auto SuperFamicomInterface::unload() -> void {
  save();
  system.unload();
}

auto SuperFamicomInterface::ports() -> vector<Port> { return {
  {ID::Port::Controller1, "Controller Port 1"},
  {ID::Port::Controller2, "Controller Port 2"},
  {ID::Port::Expansion,   "Expansion Port"   }};
}

auto SuperFamicomInterface::devices(uint port) -> vector<Device> {
  if(port == ID::Port::Controller1) return {
    {ID::Device::None,    "None"   },
    {ID::Device::Gamepad, "Gamepad"},
    {ID::Device::Mouse,   "Mouse"  }
  };

  if(port == ID::Port::Controller2) return {
    {ID::Device::None,          "None"          },
    {ID::Device::Gamepad,       "Gamepad"       },
    {ID::Device::Mouse,         "Mouse"         },
    {ID::Device::SuperMultitap, "Super Multitap"},
    {ID::Device::SuperScope,    "Super Scope"   },
    {ID::Device::Justifier,     "Justifier"     },
    {ID::Device::Justifiers,    "Justifiers"    }
  };

  if(port == ID::Port::Expansion) return {
    {ID::Device::None,        "None"       },
    {ID::Device::Satellaview, "Satellaview"},
    {ID::Device::S21FX,       "21fx"       }
  };

  return {};
}

auto SuperFamicomInterface::inputs(uint device) -> vector<Input> {
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
    {Type::Button,  "Y"     },
    {Type::Button,  "X"     },
    {Type::Trigger, "L"     },
    {Type::Trigger, "R"     },
    {Type::Control, "Select"},
    {Type::Control, "Start" }
  };

  if(device == ID::Device::Mouse) return {
    {Type::Axis,   "X-axis"},
    {Type::Axis,   "Y-axis"},
    {Type::Button, "Left"  },
    {Type::Button, "Right" }
  };

  if(device == ID::Device::SuperMultitap) {
    vector<Input> inputs;
    for(uint p = 2; p <= 5; p++) inputs.append({
      {Type::Hat,     {"Port ", p, " - ", "Up"    }},
      {Type::Hat,     {"Port ", p, " - ", "Down"  }},
      {Type::Hat,     {"Port ", p, " - ", "Left"  }},
      {Type::Hat,     {"Port ", p, " - ", "Right" }},
      {Type::Button,  {"Port ", p, " - ", "B"     }},
      {Type::Button,  {"Port ", p, " - ", "A"     }},
      {Type::Button,  {"Port ", p, " - ", "Y"     }},
      {Type::Button,  {"Port ", p, " - ", "X"     }},
      {Type::Trigger, {"Port ", p, " - ", "L"     }},
      {Type::Trigger, {"Port ", p, " - ", "R"     }},
      {Type::Control, {"Port ", p, " - ", "Select"}},
      {Type::Control, {"Port ", p, " - ", "Start" }}
    });
    return inputs;
  }

  if(device == ID::Device::SuperScope) return {
    {Type::Axis,    "X-axis" },
    {Type::Axis,    "Y-axis" },
    {Type::Control, "Trigger"},
    {Type::Control, "Cursor" },
    {Type::Control, "Turbo"  },
    {Type::Control, "Pause"  }
  };

  if(device == ID::Device::Justifier) return {
    {Type::Axis,    "X-axis" },
    {Type::Axis,    "Y-axis" },
    {Type::Control, "Trigger"},
    {Type::Control, "Start"  }
  };

  if(device == ID::Device::Justifiers) return {
    {Type::Axis,    "Port 1 - X-axis" },
    {Type::Axis,    "Port 1 - Y-axis" },
    {Type::Control, "Port 1 - Trigger"},
    {Type::Control, "Port 1 - Start"  },
    {Type::Axis,    "Port 2 - X-axis" },
    {Type::Axis,    "Port 2 - Y-axis" },
    {Type::Control, "Port 2 - Trigger"},
    {Type::Control, "Port 2 - Start"  }
  };

  if(device == ID::Device::Satellaview) return {
  };

  if(device == ID::Device::S21FX) return {
  };

  return {};
}

auto SuperFamicomInterface::connected(uint port) -> uint {
  if(port == ID::Port::Controller1) return option.port.controller1.device();
  if(port == ID::Port::Controller2) return option.port.controller2.device();
  if(port == ID::Port::Expansion) return option.port.expansion.device();
  return 0;
}

auto SuperFamicomInterface::connect(uint port, uint device) -> void {
  if(port == ID::Port::Controller1) controllerPort1.connect(option.port.controller1.device(device));
  if(port == ID::Port::Controller2) controllerPort2.connect(option.port.controller2.device(device));
  if(port == ID::Port::Expansion) expansionPort.connect(option.port.expansion.device(device));
}

auto SuperFamicomInterface::power() -> void {
  system.power(/* reset = */ false);
}

auto SuperFamicomInterface::reset() -> void {
  system.power(/* reset = */ true);
}

auto SuperFamicomInterface::run() -> void {
  system.run();
}

auto SuperFamicomInterface::rtc() -> bool {
  if(cartridge.has.EpsonRTC) return true;
  if(cartridge.has.SharpRTC) return true;
  return false;
}

auto SuperFamicomInterface::synchronize(uint64 timestamp) -> void {
  if(!timestamp) timestamp = chrono::timestamp();
  if(cartridge.has.EpsonRTC) epsonrtc.synchronize(timestamp);
  if(cartridge.has.SharpRTC) sharprtc.synchronize(timestamp);
}

auto SuperFamicomInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto SuperFamicomInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto SuperFamicomInterface::cheats(const vector<string>& list) -> void {
  cheat.reset();
  #if defined(CORE_GB)
  if(cartridge.has.ICD) return GameBoy::cheat.assign(list);
  #endif
  cheat.assign(list);
}

auto SuperFamicomInterface::options() -> Settings& {
  return option;
}

auto SuperFamicomInterface::properties() -> Settings& {
  return property;
}

}
