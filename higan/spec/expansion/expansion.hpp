struct Expansion {
  Node::Peripheral node;

  virtual auto romcs() -> bool = 0;
  virtual auto mapped(uint16 address, bool io) -> bool = 0;
  virtual auto read(uint16 address) -> uint8 = 0;
  virtual auto write(uint16 address, uint8 data) -> void = 0;
  virtual auto in(uint16 address) -> uint8 = 0;
  virtual auto out(uint16 address, uint8 data) -> void = 0;
};

#include "kempston/kempston.hpp"
#include "port.hpp"
