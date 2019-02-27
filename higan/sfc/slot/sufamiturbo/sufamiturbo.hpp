struct SufamiTurboCartridge {
  Node::Port port;
  Node::Peripheral node;
  auto load(Node::Peripheral, Node::Peripheral) -> void;

  //sufamiturbo.cpp
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;
  auto power() -> void;
  auto save() -> void;

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
    string metadata;
  } self;
};

extern SufamiTurboCartridge sufamiturboA;
extern SufamiTurboCartridge sufamiturboB;
