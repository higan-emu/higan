struct System {
  Node::Object node;

  struct Controls {
    Node::Object node;
    Node::Button up;
    Node::Button down;
    Node::Button left;
    Node::Button right;
    Node::Button b;
    Node::Button a;
    Node::Button select;
    Node::Button start;

    //controls.cpp
    auto load(Node::Object, Node::Object) -> void;
    auto poll() -> void;

    bool yHold = 0;
    bool upLatch = 0;
    bool downLatch = 0;
    bool xHold = 0;
    bool leftLatch = 0;
    bool rightLatch = 0;
  } controls;

  enum class Model : uint {
    GameBoy,
    GameBoyColor,
    SuperGameBoy,
  };
  higan::Memory::Readable<uint8> bootROM;  //todo: no higan:: prefix

  inline auto model() const -> Model { return information.model; }
  inline auto clocksExecuted() const -> uint { return information.clocksExecuted; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;
  auto clocksExecuted() -> uint;

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

  struct Information {
    Model model = Model::GameBoy;
    uint serializeSize = 0;
    uint clocksExecuted = 0;
  } information;
};

#include <gb/interface/interface.hpp>

extern System system;

auto Model::GameBoy() -> bool { return system.model() == System::Model::GameBoy; }
auto Model::GameBoyColor() -> bool { return system.model() == System::Model::GameBoyColor; }
auto Model::SuperGameBoy() -> bool { return system.model() == System::Model::SuperGameBoy; }
