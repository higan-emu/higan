#if defined(CORE_MS)

namespace higan::MasterSystem {

struct ID {
  enum : uint {
    System,
    ColecoVision,
    SG1000,
    SC3000,
    MasterSystem,
    GameGear,
  };

  struct Port { enum : uint {
    Hardware,
    Controller1,
    Controller2,
  };};

  struct Device { enum : uint {
    None,
    SG1000Controls,
    SC3000Controls,
    MasterSystemControls,
    GameGearControls,
    NumberPad,
    Gamepad,
  };};
};

struct AbstractInterface : Interface {
  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto save() -> void override;
  auto unload() -> void override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;

  auto options() -> Settings& override;
  auto properties() -> Settings& override;
};

struct ColecoVisionInterface : AbstractInterface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct SG1000Interface : AbstractInterface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct SC3000Interface : AbstractInterface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct MasterSystemInterface : AbstractInterface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
};

struct GameGearInterface : AbstractInterface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto load() -> bool override;
};

#include "options.hpp"
#include "properties.hpp"

}

#endif
