#include "display.hpp"

struct System {
  Node::Object node;
  Node::Boolean tmss;
  Node::String regionNode;
  Node::Button resetButton;
  enum class Region : uint { NTSCJ, NTSCU, PAL };

  inline auto region() const -> Region { return information.region; }
  inline auto frequency() const -> double { return information.frequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serializeInit() -> void;
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;
  auto serializeAll(serializer&) -> void;
  auto serialize(serializer&) -> void;

private:
  struct Information {
    Region region = Region::NTSCJ;
    double frequency = Constants::Colorburst::NTSC * 15.0;
    uint serializeSize = 0;
  } information;
};

extern System system;

auto Region::NTSCJ() -> bool { return system.region() == System::Region::NTSCJ; }
auto Region::NTSCU() -> bool { return system.region() == System::Region::NTSCU; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
