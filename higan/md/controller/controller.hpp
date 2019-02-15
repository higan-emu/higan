struct Controller : Thread {
  Node::Peripheral node;

  Controller();
  virtual ~Controller();

  virtual auto main() -> void;
  virtual auto readData() -> uint8 { return 0xff; }
  virtual auto writeData(uint8 data) -> void {}
};

#include "port.hpp"
#include "control-pad/control-pad.hpp"
#include "fighting-pad/fighting-pad.hpp"
