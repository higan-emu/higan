namespace Tree {
  struct System {
    auto initialize(string) -> void;
    operator Node::Object() const { return node; }
    Node::System node;
  };

  struct Display {
    auto initialize(Node::Object) -> void;
    Node::Video node;
    Node::Boolean colorEmulation;
    Node::Boolean colorBleed;
  };

  struct Speakers {
    auto initialize(Node::Object) -> void;
    Node::Audio node;
  };

  struct Hacks {
    auto initialize(Node::Object) -> void;
    Node::Object node;
    struct PPU {
      Node::Object node;
      Node::Boolean fast;
      Node::Boolean noSpriteLimit;
      Node::Boolean hiresMode7;
    } ppu;
    struct DSP {
      Node::Object node;
      Node::Boolean fast;
    } dsp;
    struct Coprocessors {
      Node::Object node;
      Node::Boolean fast;
    } coprocessors;
  };
}

extern Tree::Hacks hacks;

struct System {
  Tree::System root;
  Tree::Display display;
  Tree::Speakers speakers;

  enum class Region : uint { NTSC, PAL };

  inline auto region() const -> Region { return information.region; }
  inline auto cpuFrequency() const -> double { return information.cpuFrequency; }
  inline auto apuFrequency() const -> double { return information.apuFrequency; }

  auto run() -> void;
  auto runToSave() -> void;

  auto initialize(string) -> void;
  auto terminate() -> void;
  auto load() -> bool;
  auto save() -> void;
  auto unload() -> void;
  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize() -> serializer;
  auto unserialize(serializer&) -> bool;

private:
  struct Information {
    bool loaded = false;
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
