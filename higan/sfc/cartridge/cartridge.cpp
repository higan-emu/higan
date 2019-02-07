#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

#include "load.cpp"
#include "save.cpp"
#include "serialization.cpp"
Cartridge cartridge;

auto Cartridge::initialize(Node::Object parent) -> void {
  port = Node::Port::create("Cartridge Slot", "Cartridge");
  port->allocate = [&](auto name) {
    return Node::Peripheral::create("Cartridge");
  };
  port->attach = [&](auto node) {
    bus.reset();
    load();
    power(false);
  };
  port->detach = [&](auto node) {
    save();
    unload();
    bus.reset();
  };
  parent->append(port);
}

auto Cartridge::hashes() const -> vector<string> {
  vector<string> hashes;
  hashes.append(game.sha256);
  if(slotGameBoy.sha256) hashes.append(slotGameBoy.sha256);
  if(slotBSMemory.sha256) hashes.append(slotBSMemory.sha256);
  if(slotSufamiTurboA.sha256) hashes.append(slotSufamiTurboA.sha256);
  if(slotSufamiTurboB.sha256) hashes.append(slotSufamiTurboB.sha256);
  return hashes;
}

auto Cartridge::manifests() const -> vector<string> {
  vector<string> manifests;
  manifests.append(string{BML::serialize(game.document), "\n", BML::serialize(board)});
  if(slotGameBoy.document) manifests.append(BML::serialize(slotGameBoy.document));
  if(slotBSMemory.document) manifests.append(BML::serialize(slotBSMemory.document));
  if(slotSufamiTurboA.document) manifests.append(BML::serialize(slotSufamiTurboA.document));
  if(slotSufamiTurboB.document) manifests.append(BML::serialize(slotSufamiTurboB.document));
  return manifests;
}

auto Cartridge::titles() const -> vector<string> {
  vector<string> titles;
  titles.append(game.label);
  if(slotGameBoy.label) titles.append(slotGameBoy.label);
  if(slotBSMemory.label) titles.append(slotBSMemory.label);
  if(slotSufamiTurboA.label) titles.append(slotSufamiTurboA.label);
  if(slotSufamiTurboB.label) titles.append(slotSufamiTurboB.label);
  return titles;
}

auto Cartridge::load() -> bool {
  information = {};
  has = {};
  game = {};
  slotGameBoy = {};
  slotBSMemory = {};
  slotSufamiTurboA = {};
  slotSufamiTurboB = {};

  if(auto fp = platform->open(port->connected(), "manifest.bml", File::Read, File::Required)) {
    game.load(fp->reads());
  } else return false;
  loadCartridge(game.document);

  if(has.GameBoySlot) {
    //todo
  }

  if(has.BSMemorySlot) bsmemory.initialize(port->connected());
  if(has.SufamiTurboSlotA) sufamiturboA.initialize(port->connected());
  if(has.SufamiTurboSlotB) sufamiturboB.initialize(port->connected());

  //Game Boy
  if(cartridge.has.ICD) {
    information.sha256 = "";  //Game Boy cartridge not loaded yet: set later via loadGameBoy()
  }

  //BS Memory
  else if(cartridge.has.MCC && cartridge.has.BSMemorySlot) {
    information.sha256 = Hash::SHA256({bsmemory.memory.data(), bsmemory.memory.size()}).digest();
  }

  //Sufami Turbo
  else if(cartridge.has.SufamiTurboSlotA || cartridge.has.SufamiTurboSlotB) {
    Hash::SHA256 sha;
    if(cartridge.has.SufamiTurboSlotA) sha.input(sufamiturboA.rom.data(), sufamiturboA.rom.size());
    if(cartridge.has.SufamiTurboSlotB) sha.input(sufamiturboB.rom.data(), sufamiturboB.rom.size());
    information.sha256 = sha.digest();
  }

  //Super Famicom
  else {
    Hash::SHA256 sha;
    //hash each ROM image that exists; any with size() == 0 is ignored by sha256_chunk()
    sha.input(rom.data(), rom.size());
    sha.input(mcc.rom.data(), mcc.rom.size());
    sha.input(sa1.rom.data(), sa1.rom.size());
    sha.input(superfx.rom.data(), superfx.rom.size());
    sha.input(hitachidsp.rom.data(), hitachidsp.rom.size());
    sha.input(spc7110.prom.data(), spc7110.prom.size());
    sha.input(spc7110.drom.data(), spc7110.drom.size());
    sha.input(sdd1.rom.data(), sdd1.rom.size());
    //hash all firmware that exists
    vector<uint8> buffer;
    buffer = armdsp.firmware();
    sha.input(buffer.data(), buffer.size());
    buffer = hitachidsp.firmware();
    sha.input(buffer.data(), buffer.size());
    buffer = necdsp.firmware();
    sha.input(buffer.data(), buffer.size());
    //finalize hash
    information.sha256 = sha.digest();
  }

  return true;
}

auto Cartridge::loadGameBoy() -> bool {
  #if defined(CORE_GB)
  //invoked from ICD::load()
  information.sha256 = GameBoy::cartridge.hash();
  slotGameBoy.load(GameBoy::cartridge.manifest());
  loadCartridgeGameBoy(slotGameBoy.document);
  return true;
  #endif
  return false;
}

auto Cartridge::save() -> void {
  if(!port || !port->connected()) return;

  saveCartridge(game.document);
  if(has.GameBoySlot);  //todo
  if(has.BSMemorySlot) bsmemory.save();
  if(has.SufamiTurboSlotA) sufamiturboA.save();
  if(has.SufamiTurboSlotB) sufamiturboB.save();
}

auto Cartridge::unload() -> void {
  if(!port || !port->connected()) return;

  if(has.ICD) icd.unload();
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
  if(has.BSMemorySlot) bsmemory.unload();
  if(has.SufamiTurboSlotA) sufamiturboA.unload();
  if(has.SufamiTurboSlotB) sufamiturboB.unload();

  rom.reset();
  ram.reset();
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

}
