#include <fc/fc.hpp>

namespace higan::Famicom {

Cartridge cartridge;
#include "chip/chip.cpp"
#include "board/board.cpp"
#include "serialization.cpp"

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, "Cartridge Slot", "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  port->scan(from);
}

auto Cartridge::unload() -> void {
  port = {};
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::append<Node::Peripheral>(port, with, interface->name());

  information = {};
  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  auto document = BML::unserialize(information.manifest);
  information.name = document["game/label"].text();
  information.region = document["game/region"].text();

  Board::load(information.manifest);  //this call will set Cartridge::board if successful
  power();
  if(fds.present) {
    fds.load(node, with);
  }
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  if(fds.present) {
    fds.unload();
    fds.present = 0;
  }
  delete board;
  board = nullptr;
  node = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  board->save();
}

auto Cartridge::power() -> void {
  Thread::create(system.frequency(), {&Cartridge::main, this});
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
