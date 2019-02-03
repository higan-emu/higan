struct SufamiTurboCartridge {
  static auto create(bool slot) -> Node;
  auto initialize(Node) -> void;
  Node edge;
  Node node;

  auto load() -> void;
  auto save() -> void;
  auto unload() -> void;
  auto power() -> void;

  //memory.cpp
  auto readROM(uint24 address, uint8 data) -> uint8;
  auto writeROM(uint24 address, uint8 data) -> void;

  auto readRAM(uint24 address, uint8 data) -> uint8;
  auto writeRAM(uint24 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  ReadableMemory rom;
  WritableMemory ram;

  struct {
    string manifest;
  } self;
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
