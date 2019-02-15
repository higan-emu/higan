#include "bios.hpp"
#include "controls.hpp"
#include "display.hpp"

struct System {
  Node::Object node;
  enum class Model : uint { GameBoyAdvance, GameBoyPlayer };

  inline auto model() const -> Model { return information.model; }
  inline auto frequency() const -> double { return 16 * 1024 * 1024; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

private:
  struct Information {
    Model model = Model::GameBoyAdvance;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Model::GameBoyAdvance() -> bool { return system.model() == System::Model::GameBoyAdvance; }
auto Model::GameBoyPlayer() -> bool { return system.model() == System::Model::GameBoyPlayer; }
