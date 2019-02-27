#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

#include "load.cpp"
#include "save.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::load(Node::Object parent, Node::Object from) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
  port->allocate = [&] { return Node::Peripheral::create(interface->name()); };
  port->attach = [&](auto node) { connect(node); };
  port->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(port, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) port->connect(node);
  }
  parent->append(port);
}

auto Cartridge::connect(Node::Peripheral with) -> void {
  node = Node::Peripheral::create(interface->name());
  node->load(with);

  information = {};
  has = {};
  game = {};
  slotGameBoy = {};
  slotBSMemory = {};
  slotSufamiTurboA = {};
  slotSufamiTurboB = {};

  if(auto fp = platform->open(node, "metadata.bml", File::Read, File::Required)) {
    game.load(information.metadata = fp->reads());
  } else return;

  loadCartridge(game.document);
  if(has.GameBoySlot) icd.load(node, with);
  if(has.BSMemorySlot) bsmemory.load(node, with);
  if(has.SufamiTurboSlotA) sufamiturboA.load(node, with);
  if(has.SufamiTurboSlotB) sufamiturboB.load(node, with);

  if(has.EpsonRTC) epsonrtc.load(node, with);
  if(has.SharpRTC) sharprtc.load(node, with);

  power(false);
  port->prepend(node);
}

auto Cartridge::disconnect() -> void {
  if(!node) return;

  if(has.ICD) icd.disconnect();
  if(has.MCC) mcc.unload();
  if(has.Event) event.unload();
  if(has.SA1) sa1.unload();
  if(has.SuperFX) superfx.unload();
  if(has.ARMDSP) armdsp.unload();
  if(has.HitachiDSP) hitachidsp.unload();
  if(has.NECDSP) necdsp.unload();
  if(has.EpsonRTC) epsonrtc.unload();
  if(has.SharpRTC) sharprtc.unload();
  if(has.SPC7110) spc7110.unload();
  if(has.SDD1) sdd1.unload();
  if(has.OBC1) obc1.unload();
  if(has.MSU1) msu1.unload();
  if(has.BSMemorySlot) bsmemory.disconnect();
  if(has.SufamiTurboSlotA) sufamiturboA.disconnect();
  if(has.SufamiTurboSlotB) sufamiturboB.disconnect();

  rom.reset();
  ram.reset();
  bus.reset();
  node = {};
}

auto Cartridge::power(bool reset) -> void {
  if(has.ICD) icd.power();
  if(has.MCC) mcc.power();
  if(has.DIP) dip.power();
  if(has.Event) event.power();
  if(has.SA1) sa1.power();
  if(has.SuperFX) superfx.power();
  if(has.ARMDSP) armdsp.power();
  if(has.HitachiDSP) hitachidsp.power();
  if(has.NECDSP) necdsp.power();
  if(has.EpsonRTC) epsonrtc.power();
  if(has.SharpRTC) sharprtc.power();
  if(has.SPC7110) spc7110.power();
  if(has.SDD1) sdd1.power();
  if(has.OBC1) obc1.power();
  if(has.MSU1) msu1.power();
  if(has.BSMemorySlot) bsmemory.power();
  if(has.SufamiTurboSlotA) sufamiturboA.power();
  if(has.SufamiTurboSlotB) sufamiturboB.power();
}

auto Cartridge::save() -> void {
  if(!node) return;

  saveCartridge(game.document);
  if(has.GameBoySlot);  //todo
  if(has.BSMemorySlot) bsmemory.save();
  if(has.SufamiTurboSlotA) sufamiturboA.save();
  if(has.SufamiTurboSlotB) sufamiturboB.save();
}

}
