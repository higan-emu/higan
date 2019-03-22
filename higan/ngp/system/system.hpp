#include "controls.hpp"
#include "display.hpp"

struct System {
  Node::Object node;

  enum class Model : uint { NeoGeoPocket, NeoGeoPocketColor };
  Memory::Readable<uint8> bios;

  inline auto model() const -> Model { return information.model; }
  inline auto frequency() const -> double { return 6'144'000; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

private:
  struct Information {
    Model model = Model::NeoGeoPocket;
    natural serializeSize;
  } information;
};

extern System system;

auto Model::NeoGeoPocket() -> bool { return system.model() == System::Model::NeoGeoPocket; }
auto Model::NeoGeoPocketColor() -> bool { return system.model() == System::Model::NeoGeoPocketColor; }
