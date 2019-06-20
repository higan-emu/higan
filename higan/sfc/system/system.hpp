extern Random random;
extern Cheat cheat;

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

  struct Video {
    Node::Video node;
    Node::Boolean colorEmulation;
    Node::Boolean colorBleed;

    //video.cpp
    auto load(Node::Object, Node::Object) -> void;
    auto color(uint32) -> uint64;
  } video;

  enum class Region : uint { NTSC, PAL };

  inline auto region() const -> Region { return information.region; }
  inline auto cpuFrequency() const -> double { return information.cpuFrequency; }
  inline auto apuFrequency() const -> double { return information.apuFrequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object) -> void;
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
  } information;

  uint serializeSize = 0;

  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&) -> void;
  auto serializeInit() -> void;

  friend class Cartridge;
};

extern System system;

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
