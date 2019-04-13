#include <msx/msx.hpp>

namespace higan::MSX {

Keyboard keyboard;
#include "serialization.cpp"

auto Keyboard::load(Node::Object parent, Node::Object from) -> void {
}

auto Keyboard::power() -> void {
  io = {};
}

auto Keyboard::read() -> uint8 {
  uint8 index = io.select << 3;
  uint8 data = 0xff;
  return data;
}

auto Keyboard::write(uint4 data) -> void {
  io.select = data;
}

}
