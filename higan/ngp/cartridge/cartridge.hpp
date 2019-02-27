//Toshiba  0x98
//Sharp    0xb0
//Samsung  0xec

// 4mbit   0xab
// 8mbit   0x2c
//16mbit   0x2f

struct Flash {
  natural ID;  //todo: can this be made const, even though it's declared as Cartridge::Flash[2] ?

  Memory::Writable<uint8> rom;
  boolean modified;
  uint8 vendorID;
  uint8 deviceID;

  struct Block {
    boolean writable;
    natural offset;
    natural length;
  };
  vector<Block> blocks;

  explicit operator bool() const { return (bool)rom; }

  //flash.cpp
  auto reset(natural ID) -> void;
  auto allocate(natural size) -> bool;
  auto load(vfs::shared::file fp) -> void;

  auto power() -> void;
  auto read(uint21 address) -> uint8;
  auto write(uint21 address, uint8 data) -> void;

  auto status(uint) -> void;
  auto program(uint21 address, uint8 data) -> void;
  auto erase(uint6 blockID) -> void;
  auto eraseAll() -> void;
  auto protect(uint6 blockID) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  enum : uint { Read, Prefix, Suffix, ExtendedPrefix, ExtendedSuffix, ReadID, Write };
  natural mode;
};

struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  Flash flash[2];

  inline auto metadata() const -> string { return information.metadata; }

  //cartridge.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;
  auto save() -> void;
  auto power() -> void;

  auto read(uint1 bank, uint21 address) -> uint8;
  auto write(uint1 bank, uint21 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct Information {
    string metadata;
  } information;
};

extern Cartridge cartridge;
