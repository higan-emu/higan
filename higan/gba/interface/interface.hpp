#if defined(CORE_GBA)

namespace higan::GameBoyAdvance {

struct ID {
  enum : uint {
    System,
    GameBoyAdvance,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    Controls,
  };};
};

struct GameBoyAdvanceInterface : Interface {
  auto information() -> Information override;

  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto slots() -> vector<Slot> override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto load() -> bool override;
  auto load(uint id) -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto options() -> Settings& override;
  auto properties() -> Settings& override;
};

#include "options.hpp"
#include "properties.hpp"

}

#endif
