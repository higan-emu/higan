struct Controller : Thread {
  Node::Peripheral node;

  Controller();
  virtual ~Controller();

  auto main() -> void;

  virtual auto read() -> uint8 { return 0xff; }
  virtual auto write(uint8 data) -> void {}
};

#include "port.hpp"
#include "numberpad/numberpad.hpp"
#include "gamepad/gamepad.hpp"
