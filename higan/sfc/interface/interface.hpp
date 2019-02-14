#if defined(CORE_SFC)

namespace higan::SuperFamicom {

extern Interface* interface;

struct SuperFamicomInterface : Interface {
  auto name() -> string override;

  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>&) -> void override;
};

}

#endif
