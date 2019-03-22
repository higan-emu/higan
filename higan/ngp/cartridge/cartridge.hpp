#include "flash.hpp"

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
