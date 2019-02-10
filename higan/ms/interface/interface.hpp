#if defined(CORE_MS)

namespace higan::MasterSystem {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto initialize(string configuration = {}) -> void override;
  auto terminate() -> void override;
  auto root() -> Node::Object override;

  auto power() -> void override;
  auto run() -> void override;
  auto save() -> void override;
  auto unload() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;
};

struct ColecoVisionInterface : AbstractInterface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;
};

struct SG1000Interface : AbstractInterface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;
};

struct SC3000Interface : AbstractInterface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;
};

struct MasterSystemInterface : AbstractInterface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;
};

struct GameGearInterface : AbstractInterface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;
};

}

#endif
