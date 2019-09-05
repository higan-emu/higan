extern Random random;

struct System {
  Node::Object node;
  Node::String regionNode;

  struct Controls {
    Node::Object node;
    Node::Button reset;

    //controls.cpp
    auto load(Node::Object, Node::Object) -> void;
    auto poll() -> void;
  } controls;

  enum class Region : uint { NTSC, PAL };

  inline auto region() const -> Region { return information.region; }
  inline auto cpuFrequency() const -> double { return information.cpuFrequency; }
  inline auto apuFrequency() const -> double { return information.apuFrequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object&, Node::Object) -> void;
  auto unload() -> void;
  auto save() -> void;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

private:
  struct Information {
    Region region = Region::NTSC;
    double cpuFrequency = Constants::Colorburst::NTSC * 6.0;
    double apuFrequency = 32040.0 * 768.0;
    uint32 serializeSize;
  } information;

  //serialization.cpp
  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  friend class Cartridge;
};

extern System system;

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
