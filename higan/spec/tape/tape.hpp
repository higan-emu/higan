struct Tape : Thread {
  Node::Peripheral node;
  Node::Stream stream;

  auto manifest() const -> string { return information.manifest; }
  auto name() const -> string { return information.name; }

  //tape.cpp
  auto allocate(Node::Port) -> Node::Peripheral;
  auto connect() -> void;
  auto disconnect() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;

  auto save() -> void;

private:
  struct Information {
    string manifest;
    string name;
  } information;

  uint64 position;
  uint64 length;
  uint64 range;
  uint64 frequency;
  Memory::Writable<uint64> data;

};

#include "tray.hpp"
#include "deck.hpp"
