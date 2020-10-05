struct ROM {
  Memory::Readable<uint8> bios;
  Memory::Readable<uint8> sub;
};

struct System {
  Node::Object node;
  Node::String regionNode;

  enum class Model : uint { Spectrum48k, Spectrum128 };
  enum class Region : uint { PAL };

  auto model() const -> Model { return information.model; }
  auto region() const -> Region { return information.region; }
  auto frequency() const -> double { return information.frequency; }

  //system.cpp
  auto run() -> void;

  auto load(Node::Object&) -> void;
  auto save() -> void;
  auto unload() -> void;

  auto power() -> void;

  //serialization.cpp
  auto serialize(bool synchronize) -> serializer;
  auto unserialize(serializer&) -> bool;

  // 128k specific state
  bool romBank;
  bool screenBank;
  uint3 ramBank;
  bool pagingDisabled;
private:
  struct Information {
    Model model = Model::Spectrum48k;
    Region region = Region::PAL;
    double frequency = 3'500'000;
    uint32 serializeSize[2];
  } information;

  //serialization.cpp
  auto serialize(serializer&) -> void;
  auto serializeAll(serializer&, bool synchronize) -> void;
  auto serializeInit(bool synchronize) -> uint;
};

extern ROM rom;
extern System system;

auto Model::Spectrum48k() -> bool { return system.model() == System::Model::Spectrum48k; }
auto Model::Spectrum128() -> bool { return system.model() == System::Model::Spectrum128; }

auto Region::PAL() -> bool { return system.region() == System::Region::PAL; }
