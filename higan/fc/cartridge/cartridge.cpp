#include <fc/fc.hpp>

namespace higan::Famicom {

Cartridge cartridge;
#include "chip/chip.cpp"
#include "board/board.cpp"
#include "serialization.cpp"

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create("Cartridge", port->type);
  node->load(with);

  information = {};
  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  Board::load(information.manifest);  //this call will set Cartridge::board if successful
  power();
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  delete board;
  board = nullptr;
  node = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  board->save();
}

auto Cartridge::power() -> void {
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  board->power();
}

auto Cartridge::main() -> void {
  board->main();
}

auto Cartridge::readPRG(uint addr) -> uint8 {
  return board->readPRG(addr);
}

auto Cartridge::writePRG(uint addr, uint8 data) -> void {
  return board->writePRG(addr, data);
}

auto Cartridge::readCHR(uint addr) -> uint8 {
  return board->readCHR(addr);
}

auto Cartridge::writeCHR(uint addr, uint8 data) -> void {
  return board->writeCHR(addr, data);
}

auto Cartridge::scanline(uint y) -> void {
  return board->scanline(y);
}

}
