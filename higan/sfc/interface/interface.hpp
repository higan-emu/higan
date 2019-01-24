#if defined(CORE_SFC)

namespace higan::SuperFamicom {

struct ID {
  enum : uint {
    System,
    SuperFamicom,
    GameBoy,
    BSMemory,
    SufamiTurboA,
    SufamiTurboB,
  };

  struct Port { enum : uint {
    Controller1,
    Controller2,
    Expansion,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
    Mouse,
    SuperMultitap,
    SuperScope,
    Justifier,
    Justifiers,

    Satellaview,
    S21FX,
  };};
};

struct SuperFamicomInterface : Interface {
  auto information() -> Information;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto load() -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto rtc() -> bool override;
  auto synchronize(uint64 timestamp) -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto properties() -> AbstractSetting* override;
  auto settings() -> AbstractSetting* override;

  auto cheats(const vector<string>&) -> void override;

  auto configuration() -> string override;
  auto configuration(string name) -> string override;
  auto configure(string configuration) -> bool override;
  auto configure(string name, string value) -> bool override;
};

#include "configuration.hpp"

struct Properties : Setting<string> {
  Properties() : Setting{"system", "Super Famicom"} {}

  struct CPU : Setting<> { using Setting::Setting;
    Setting<natural> version{self, "version", 2};
  } cpu{self, "cpu"};

  struct PPU1 : Setting<> { using Setting::Setting;
    Setting<natural> version{self, "version", 1};
    struct VRAM : Setting<> { using Setting::Setting;
      Setting<natural> size{self, "size", 0x10000};
    } vram{self, "vram"};
  } ppu1{self, "ppu1"};

  struct PPU2 : Setting<> { using Setting::Setting;
    Setting<natural> version{self, "version", 3};
  } ppu2{self, "ppu2"};
};

struct Settings : Setting<> {
  Settings() : Setting{"settings"} {}

  uint controllerPort1 = ID::Device::Gamepad;
  uint controllerPort2 = ID::Device::Gamepad;
  uint expansionPort = ID::Device::None;
  bool random = true;
};

extern Properties properties;
extern Settings settings;

}

#endif
