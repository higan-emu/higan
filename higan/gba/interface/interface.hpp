#if defined(CORE_GBA)

namespace higan::GameBoyAdvance {

extern Interface* interface;

struct GameBoyAdvanceInterface : Interface {
  auto name() -> string override { return "Game Boy Advance"; }
  auto game() -> string override;

  auto root() -> Node::Object override;
  auto load(Node::Object&, string tree = {}) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;
};

struct GameBoyPlayerInterface : GameBoyAdvanceInterface {
  auto name() -> string override { return "Game Boy Player"; }
};

}

#endif
