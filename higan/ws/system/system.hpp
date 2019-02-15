#include "controls.hpp"
#include "display.hpp"

struct System : IO {
  Node::Object node;
  enum class Model : uint { WonderSwan, WonderSwanColor, SwanCrystal, PocketChallengeV2 };

  inline auto model() const -> Model { return information.model; }
  inline auto color() const -> bool { return r.color; }
  inline auto planar() const -> bool { return r.format == 0; }
  inline auto packed() const -> bool { return r.format == 1; }
  inline auto depth() const -> bool { return r.color && r.depth == 1; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power() -> void;

  //io.cpp
  auto portRead(uint16 addr) -> uint8 override;
  auto portWrite(uint16 addr, uint8 data) -> void override;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

  struct Information {
    Model model = Model::WonderSwan;
    uint serializeSize = 0;
  } information;

  EEPROM eeprom;

private:
  struct Registers {
    //$0060  DISP_MODE
    uint5 unknown;
    uint1 format;
    uint1 depth;
    uint1 color;
  } r;
};

extern System system;

auto Model::WonderSwan() -> bool { return system.model() == System::Model::WonderSwan; }
auto Model::WonderSwanColor() -> bool { return system.model() == System::Model::WonderSwanColor; }
auto Model::SwanCrystal() -> bool { return system.model() == System::Model::SwanCrystal; }
auto Model::PocketChallengeV2() -> bool { return system.model() == System::Model::PocketChallengeV2; }
