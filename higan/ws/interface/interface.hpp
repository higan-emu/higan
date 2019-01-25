#if defined(CORE_WS)

namespace higan::WonderSwan {

struct ID {
  enum : uint {
    System,
    WonderSwan,
    WonderSwanColor,
    PocketChallengeV2,
  };

  struct Port { enum : uint {
    Hardware,
  };};

  struct Device { enum : uint {
    Controls,
  };};
};

struct AbstractInterface : Interface {
  auto display() -> Display override;
  auto color(uint32 color) -> uint64 override;

  auto ports() -> vector<Port> override;
  auto devices(uint ports) -> vector<Device> override;
  auto inputs(uint devices) -> vector<Input> override;

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
};

struct WonderSwanInterface : AbstractInterface {
  WonderSwanInterface();

  auto information() -> Information override;

  auto load() -> bool override;

  auto properties() -> Settings& override;
};

struct WonderSwanColorInterface : AbstractInterface {
  WonderSwanColorInterface();

  auto information() -> Information override;

  auto load() -> bool override;

  auto properties() -> Settings& override;
};

struct PocketChallengeV2Interface : AbstractInterface {
  PocketChallengeV2Interface();

  auto information() -> Information override;

  auto load() -> bool override;

  auto properties() -> Settings& override;
};

#include "options.hpp"
#include "properties.hpp"

}

#endif
