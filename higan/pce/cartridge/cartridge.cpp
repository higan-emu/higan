#include <pce/pce.hpp>

namespace higan::PCEngine {

Cartridge cartridge;
#include "board/board.cpp"
#include "serialization.cpp"

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::append<Node::Port>(parent, from, "Cartridge Slot");
  port->setFamily(interface->name());
  port->setType("Cartridge");
  port->setAllocate([&] { return Node::Peripheral::create(interface->name()); });
  port->setAttach([&](auto node) { connect(node); });
  port->setDetach([&](auto node) { disconnect(); });
  port->scan(from);
}

//most PC Engine HuCards lack save RAM on them due to the card size and cost savings.
//the PC Engine CD adds 2KB of backup RAM that some HuCard games can use for saves.
//however, all games must share this small amount of RAM.
//since this is an emulator, we can make this process nicer by storing BRAM per-game.

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::append<Node::Peripheral>(port, with, interface->name());
  node->setManifest([&] { return information.manifest; });

  information = {};

  if(auto fp = platform->open(node, "manifest.bml", File::Read, File::Required)) {
    information.manifest = fp->reads();
  }

  auto document = BML::unserialize(information.manifest);
  information.name = document["game/label"].string();
  information.board = document["game/board"].string();

  if(information.board == "Linear") board = new Board::Linear;
  if(information.board == "Split" ) board = new Board::Split;
  if(information.board == "Banked") board = new Board::Banked;
  if(!board) board = new Board::Interface;
  board->load(document);

  if(auto fp = platform->open(node, "save.ram", File::Read)) {
    cpu.bram.load(fp);
  }

  power();
}

auto Cartridge::disconnect() -> void {
  if(!node) return;
  node = {};
  board = {};
}

auto Cartridge::save() -> void {
  if(!node) return;
  auto document = BML::unserialize(information.manifest);
  board->save(document);

  if(auto fp = platform->open(node, "save.ram", File::Write)) {
    cpu.bram.save(fp);
  }
}

auto Cartridge::power() -> void {
  board->power();
}

auto Cartridge::read(uint8 bank, uint13 address) -> uint8 {
  return board->read(bank, address);
}

auto Cartridge::write(uint8 bank, uint13 address, uint8 data) -> void {
  return board->write(bank, address, data);
}

}
