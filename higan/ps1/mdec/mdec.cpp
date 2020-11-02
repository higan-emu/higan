#include <ps1/ps1.hpp>

namespace higan::PlayStation {

MDEC mdec;
#include "io.cpp"
#include "serialization.cpp"
#include "macroblock.cpp"
#include "rle.cpp"
#include "idct.cpp"
#include "yuvtorgb.cpp"
#include "ytograyscale.cpp"

auto MDEC::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("MDEC");
  fifo.mdecIn.resize(4*32);
}

auto MDEC::unload() -> void {
  fifo.mdecOut.reset();
  fifo.mdecIn.reset();
  node.reset();
}

auto MDEC::power(bool reset) -> void {
  Thread::reset();
  io = {};
}

}
