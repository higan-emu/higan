struct System {
  Node::Object root;
  Node::Button resetButton;
  enum class Region : uint { NTSC, PAL };

  inline auto region() const -> Region { return information.region; }
  inline auto cpuFrequency() const -> double { return information.cpuFrequency; }
  inline auto apuFrequency() const -> double { return information.apuFrequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto load(Node::Object from) -> void;
  auto save() -> void;
  auto unload() -> void;
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

struct Display {
  Node::Video node;
  Node::Boolean colorEmulation;
  Node::Boolean colorBleed;

  auto load(Node::Object parent, Node::Object from) -> void;
  auto color(natural) -> uint64_t;
};

struct Speakers {
  Node::Audio node;

  auto load(Node::Object parent, Node::Object from) -> void;
};

struct Hacks {
  Node::Settings node;
  struct PPU {
    Node::Boolean fast;
    Node::Boolean noSpriteLimit;
    Node::Boolean hiresMode7;
  } ppu;
  struct DSP {
    Node::Boolean fast;
  } dsp;
  struct Coprocessors {
    Node::Boolean fast;
  } coprocessors;

  auto load(Node::Object parent, Node::Object from) -> void;
};

extern System system;
extern Display display;
extern Speakers speakers;
extern Hacks hacks;

auto Region::NTSC() -> bool { return system.region() == System::Region::NTSC; }
auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
