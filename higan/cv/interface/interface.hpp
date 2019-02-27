#if defined(CORE_CV)

namespace higan::ColecoVision {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto root() -> Node::Object override;
  auto load(string tree = {}) -> void override;
  auto power() -> void override;
  auto run() -> void override;
  auto save() -> void override;
  auto unload() -> void override;

  auto serialize() -> serializer override;
  auto unserialize(serializer&) -> bool override;

  auto cheats(const vector<string>& list) -> void override;
};

struct ColecoVisionInterface : AbstractInterface {
  auto name() -> string override { return "ColecoVision"; }
};

//note: not emulated at all yet ...
//struct ColecoAdamInterface : AbstractInterface {
//  auto name() -> string override { return "ColecoAdam"; }
//};

}

#endif
