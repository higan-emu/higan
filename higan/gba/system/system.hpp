#include "bios.hpp"

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
    Node::Button l;
    Node::Button r;
    Node::Button select;
    Node::Button start;
    Node::Rumble rumbler;  //Game Boy Player

    auto load(Node::Object, Node::Object) -> void;
    auto poll() -> void;
    auto rumble(bool enable) -> void;

    bool yHold = 0;
    bool upLatch = 0;
    bool downLatch = 0;
    bool xHold = 0;
    bool leftLatch = 0;
    bool rightLatch = 0;
  } controls;

  struct Video {
    Node::Video node;
    Node::Boolean colorEmulation;
    Node::Boolean interframeBlending;
    Node::String orientation;

    //video.cpp
    auto load(Node::Object, Node::Object) -> void;
    auto color(uint32) -> uint64;
  } video;

  enum class Model : uint { GameBoyAdvance, GameBoyPlayer };

  inline auto model() const -> Model { return information.model; }
  inline auto frequency() const -> double { return 16 * 1024 * 1024; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto save() -> void;
  auto unload() -> void;
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
