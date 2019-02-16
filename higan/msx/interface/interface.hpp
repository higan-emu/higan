#if defined(CORE_MSX)

namespace higan::MSX {

extern Interface* interface;

struct MSXInterface : Interface {
  auto name() -> string override { return "MSX"; }

  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;
};

}

#endif
