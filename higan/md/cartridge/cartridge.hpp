struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  Memory::Readable<uint8> rom;
  Memory::Readable<uint8> patch;
  Memory::Writable<uint8> ram;

  inline auto metadata() const -> string { return information.metadata; }
  inline auto region() const -> string { return information.region; }

  //cartridge.cpp
  Cartridge() = default;
  Cartridge(uint depth) : depth(depth) {}

  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto loadROM(Memory::Readable<uint8>& rom, Markup::Node memory) -> bool;
  auto loadRAM(Memory::Writable<uint8>& ram, Markup::Node memory) -> bool;
  auto saveRAM(Memory::Writable<uint8>& ram, Markup::Node memory) -> bool;

  enum : uint { zero = 0 };
  auto readIO(uint1 size, uint24 address, uint16 data) -> uint16;
  auto writeIO(uint1 size, uint24 address, uint16 data) -> void;
  auto writeIO(uint24 address, uint8 data) -> void;

  auto readLinear(uint1 size, uint22 address, uint16 data) -> uint16;
  auto writeLinear(uint1 size, uint22 address, uint16 data) -> void;

  auto readBanked(uint1 size, uint22 address, uint16 data) -> uint16;
  auto writeBanked(uint1 size, uint22 address, uint16 data) -> void;

  auto readLockOn(uint1 size, uint22 address, uint16 data) -> uint16;
  auto writeLockOn(uint1 size, uint22 address, uint16 data) -> void;

  auto readGameGenie(uint1 size, uint22 address, uint16 data) -> uint16;
  auto writeGameGenie(uint1 size, uint22 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Information {
    string metadata;
    string region;
  } information;

  uint16 ramBits;
   uint1 ramEnable;
   uint1 ramWritable;
   uint6 romBank[8];

  struct GameGenie {
    boolean enable;
    struct Code {
      boolean enable;
      uint24 address;
      uint16 data;
    } codes[5];
  } gameGenie;

  function<uint16 (uint1 size, uint22 address, uint16 data)> read;
  function<void (uint1 size, uint22 address, uint16 data)> write;

  unique_pointer<Cartridge> slot;
  const uint depth = 0;
};

extern Cartridge cartridge;
