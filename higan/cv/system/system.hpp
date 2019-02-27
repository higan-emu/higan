#include "controls.hpp"
#include "display.hpp"

struct System {
  Node::Object node;
  Node::String regionNode;

  enum class Model : uint { ColecoVision, ColecoAdam };
  enum class Region : uint { NTSC, PAL };

  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }
  auto colorburst() const -> double { return information.colorburst; }

  //system.cpp
  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

  uint8 bios[0x2000];

private:
  struct Information {
    Model model = Model::ColecoVision;
    Region region = Region::NTSC;
    double colorburst = Constants::Colorburst::NTSC;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Model::ColecoVision() -> bool { return system.model() == System::Model::ColecoVision; }
auto Model::ColecoAdam() -> bool { return system.model() == System::Model::ColecoAdam; }

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
