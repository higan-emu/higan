#include "display.hpp"

struct System {
  Node::Object node;
  enum class Model : uint { PCEngine, SuperGrafx };

  inline auto model() const -> Model { return information.model; }
  inline auto colorburst() const -> double { return information.colorburst; }

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
    Model model = Model::PCEngine;
    double colorburst = Constants::Colorburst::NTSC;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Model::PCEngine() -> bool { return system.model() == System::Model::PCEngine; }
auto Model::SuperGrafx() -> bool { return system.model() == System::Model::SuperGrafx; }
