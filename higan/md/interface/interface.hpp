#if defined(CORE_MD)

namespace higan::MegaDrive {

extern Interface* interface;

struct MegaDriveInterface : Interface {
  auto name() -> string override { return "Mega Drive"; }

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

}

#endif
