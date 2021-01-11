#if defined(CORE_SPEC)

namespace higan::Spectrum {

extern Interface* interface;

struct AbstractInterface : Interface {
  auto game() -> string override;

  auto root() -> Node::Object override;
  auto load(Node::Object&) -> void override;
  auto unload() -> void override;
  auto save() -> void override;
  auto power() -> void override;
  auto run() -> void override;

  auto serialize(bool synchronize) -> serializer override;
  auto unserialize(serializer&) -> bool override;
};

struct Spectrum48kInterface : AbstractInterface {
  auto name() -> string override { return "ZX Spectrum 48k"; }
};

struct Spectrum128Interface : AbstractInterface {
  auto name() -> string override { return "ZX Spectrum 128"; }
};

}

#endif
