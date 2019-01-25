#if defined(CORE_PCE)

namespace higan::PCEngine {

struct ID {
  enum : uint {
    System,
    PCEngine,
    SuperGrafx,
  };

  struct Port { enum : uint {
    Controller,
  };};

  struct Device { enum : uint {
    None,
    Gamepad,
  };};
};

struct AbstractInterface : Interface {
  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto loaded() -> bool override;
  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto save() -> void override;
  auto unload() -> void override;

  auto ports() -> vector<Port> override;
  auto devices(uint port) -> vector<Device> override;
  auto inputs(uint device) -> vector<Input> override;

  auto connected(uint port) -> uint override;
  auto connect(uint port, uint device) -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;

  auto options() -> Settings&;
};

struct PCEngineInterface : AbstractInterface {
  auto information() -> Information override;

  auto load() -> bool override;

  auto properties() -> Settings&;
};

struct SuperGrafxInterface : AbstractInterface {
  auto information() -> Information override;

  auto load() -> bool override;

  auto properties() -> Settings&;
};

#include "options.hpp"
#include "properties.hpp"

}

#endif
