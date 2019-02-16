#if defined(CORE_PCE)

namespace higan::PCEngine {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;
};

struct PCEngineInterface : AbstractInterface {
  auto name() -> string override { return "PC Engine"; }
};

struct SuperGrafxInterface : AbstractInterface {
  auto name() -> string override { return "SuperGrafx"; }
};

}

#endif
