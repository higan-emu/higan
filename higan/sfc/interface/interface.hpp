#if defined(CORE_SFC)

namespace higan::SuperFamicom {

extern Interface* interface;

struct SuperFamicomInterface : Interface {
  auto name() -> string override;
  auto colors() -> uint32 override;
  auto color(uint32 color) -> uint64 override;

  auto hashes() -> vector<string> override;
  auto manifests() -> vector<string> override;
  auto titles() -> vector<string> override;
  auto load() -> bool override;
  auto load(uint slot) -> bool override;
  auto save() -> void override;
  auto unload() -> void override;

  auto initialize(string configuration = {}) -> void override;
  auto terminate() -> void override;
  auto root() -> Node::Object override;

  auto power() -> void override;
  auto reset() -> void override;
  auto run() -> void override;

  auto rtc() -> bool override;
  auto synchronize(uint64 timestamp) -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>&) -> void override;
};

}

#endif
