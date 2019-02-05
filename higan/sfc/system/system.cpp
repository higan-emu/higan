#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

auto Tree::System::initialize(string configuration) -> void {
  node = Node::System::create("Super Famicom");
  node->copy(Node::unserialize(configuration));  //temporary hack (load needs system("location") for IPLROM)
  cartridge.initialize(node);
  controllerPort1.initialize(node);
  controllerPort2.initialize(node);
  expansionPort.initialize(node);
  system.display.initialize(node);
  system.speakers.initialize(node);
  cpu.initialize(node);
  ppu.nodes.ppu1.initialize(node);
  ppu.nodes.ppu2.initialize(node);
  ppu.nodes.vram.initialize(node);
  hacks.initialize(node);
  system.load();
  node->copy(Node::unserialize(configuration));
}

auto Tree::Display::initialize(Node::Object parent) -> void {
  node = Node::Video::create("Display");
  node->type   = "CRT";
  node->width  = 512;
  node->height = 480;
  node->aspect = 8.0 / 7.0;
  node->append(colorEmulation = Node::Boolean::create("Color Emulation", true, [&](auto) {
    video.setPalette();
  }));
  node->append(colorBleed = Node::Boolean::create("Color Bleed", true, [&](auto value) {
    video.setEffect(higan::Video::Effect::ColorBleed, value);
  }));
  parent->append(node);
}

auto Tree::Speakers::initialize(Node::Object parent) -> void {
  node = Node::Audio::create("Speakers");
  parent->append(node);
}

auto Tree::Hacks::initialize(Node::Object parent) -> void {
  parent->append(node = Node::Object::create("Hacks"));
  node->append(ppu.node = Node::Object::create("PPU"));
  ppu.node->append(ppu.fast = Node::Boolean::create("Fast", false));
  ppu.node->append(ppu.noSpriteLimit = Node::Boolean::create("No Sprite Limit", false));
  ppu.node->append(ppu.hiresMode7 = Node::Boolean::create("Hires Mode 7", false));
  node->append(dsp.node = Node::Object::create("DSP"));
  dsp.node->append(dsp.fast = Node::Boolean::create("Fast", false));
  node->append(coprocessors.node = Node::Object::create("Coprocessors"));
  coprocessors.node->append(coprocessors.fast = Node::Boolean::create("Fast", false));
}

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
Tree::Hacks hacks;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(smp);
  scheduler.synchronize(ppu);
  scheduler.synchronize(dsp);
  for(auto coprocessor : cpu.coprocessors) scheduler.synchronize(*coprocessor);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::initialize(string configuration) -> void {
  root.initialize(configuration);
}

auto System::terminate() -> void {
  root.node->reset();
  root = {};
}

auto System::load() -> bool {
  information = {};

  //these values cannot change at run-time
  hacks.ppu.fast->setLatch();
  hacks.ppu.noSpriteLimit->setLatch();
  hacks.ppu.hiresMode7->setLatch();
  hacks.dsp.fast->setLatch();
  hacks.coprocessors.fast->setLatch();

  bus.reset();
  if(!cpu.load()) return false;
  if(!smp.load()) return false;
  if(!ppu.load()) return false;
  if(!dsp.load()) return false;

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.cpuFrequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.cpuFrequency = Constants::Colorburst::PAL * 4.8;
  }

  if(cartridge.has.ICD) icd.load();
  if(cartridge.has.BSMemorySlot) bsmemory.load();

  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  cpu.peripherals.reset();
  controllerPort1.unload();
  controllerPort2.unload();
  expansionPort.unload();

  if(cartridge.has.ICD) icd.unload();
  if(cartridge.has.MCC) mcc.unload();
  if(cartridge.has.Event) event.unload();
  if(cartridge.has.SA1) sa1.unload();
  if(cartridge.has.SuperFX) superfx.unload();
  if(cartridge.has.HitachiDSP) hitachidsp.unload();
  if(cartridge.has.SPC7110) spc7110.unload();
  if(cartridge.has.SDD1) sdd1.unload();
  if(cartridge.has.OBC1) obc1.unload();
  if(cartridge.has.MSU1) msu1.unload();
  if(cartridge.has.BSMemorySlot) bsmemory.unload();
  if(cartridge.has.SufamiTurboSlotA) sufamiturboA.unload();
  if(cartridge.has.SufamiTurboSlotB) sufamiturboB.unload();

  cartridge.unload();
  information.loaded = false;
}

auto System::power(bool reset) -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  scheduler.reset();
  cpu.power(reset);
  smp.power(reset);
  dsp.power(reset);
  ppu.power(reset);

  if(cartridge.has.ICD) icd.power();
  if(cartridge.has.MCC) mcc.power();
  if(cartridge.has.DIP) dip.power();
  if(cartridge.has.Event) event.power();
  if(cartridge.has.SA1) sa1.power();
  if(cartridge.has.SuperFX) superfx.power();
  if(cartridge.has.ARMDSP) armdsp.power();
  if(cartridge.has.HitachiDSP) hitachidsp.power();
  if(cartridge.has.NECDSP) necdsp.power();
  if(cartridge.has.EpsonRTC) epsonrtc.power();
  if(cartridge.has.SharpRTC) sharprtc.power();
  if(cartridge.has.SPC7110) spc7110.power();
  if(cartridge.has.SDD1) sdd1.power();
  if(cartridge.has.OBC1) obc1.power();
  if(cartridge.has.MSU1) msu1.power();
  if(cartridge.has.BSMemorySlot) bsmemory.power();
  if(cartridge.has.SufamiTurboSlotA) sufamiturboA.power();
  if(cartridge.has.SufamiTurboSlotB) sufamiturboB.power();

  if(cartridge.has.ICD) cpu.coprocessors.append(&icd);
  if(cartridge.has.Event) cpu.coprocessors.append(&event);
  if(cartridge.has.SA1) cpu.coprocessors.append(&sa1);
  if(cartridge.has.SuperFX) cpu.coprocessors.append(&superfx);
  if(cartridge.has.ARMDSP) cpu.coprocessors.append(&armdsp);
  if(cartridge.has.HitachiDSP) cpu.coprocessors.append(&hitachidsp);
  if(cartridge.has.NECDSP) cpu.coprocessors.append(&necdsp);
  if(cartridge.has.EpsonRTC) cpu.coprocessors.append(&epsonrtc);
  if(cartridge.has.SharpRTC) cpu.coprocessors.append(&sharprtc);
  if(cartridge.has.SPC7110) cpu.coprocessors.append(&spc7110);
  if(cartridge.has.MSU1) cpu.coprocessors.append(&msu1);
  if(cartridge.has.BSMemorySlot) cpu.coprocessors.append(&bsmemory);

  scheduler.primary(cpu);

  controllerPort1.power(ID::Port::Controller1);
  controllerPort2.power(ID::Port::Controller2);
  expansionPort.power();
}

}
