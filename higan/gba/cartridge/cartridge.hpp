struct Cartridge {
  Node::Port port;
  Node::Peripheral node;

  #include "memory.hpp"

  inline auto metadata() const -> string { return information.metadata; }

  //cartridge.cpp
  Cartridge();
  ~Cartridge();

  auto load(Node::Object, Node::Object) -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;

  auto save() -> void;
  auto power() -> void;

  auto read(uint mode, uint32 addr) -> uint32;
  auto write(uint mode, uint32 addr, uint32 word) -> void;

  auto serialize(serializer&) -> void;

private:
  struct {
    string metadata;
  } information;

  bool hasSRAM = false;
  bool hasEEPROM = false;
  bool hasFLASH = false;
};

extern Cartridge cartridge;
