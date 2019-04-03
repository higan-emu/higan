struct Controller : Thread {
  Controller();
  virtual ~Controller();

  auto main() -> void;

  virtual auto read() -> uint6 { return 0x3f; }
  virtual auto write(uint8 data) -> void {}

protected:
  Node::Peripheral node;
};

#include "port.hpp"
#include "gamepad/gamepad.hpp"
