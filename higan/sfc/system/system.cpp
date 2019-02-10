#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

System system;
Display display;
Speakers speakers;
Hacks hacks;
Scheduler scheduler;
Random random;
Cheat cheat;
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

auto System::load(Node::Object from) -> void {
  if(root) {
    save();
    unload();
    bus.reset();
  }

  root = Node::System::create("Super Famicom");
  root->load(from);
  hacks.load(root, from);
  cartridge.load(root, from);
  controllerPort1.load(root, from);
  controllerPort2.load(root, from);
  expansionPort.load(root, from);
  display.load(root, from);
  speakers.load(root, from);
  cpu.load(root, from);
  ppu.load(root, from);

  information = {};

  //these values cannot change at run-time
  hacks.ppu.fast->setLatch();
  hacks.ppu.noSpriteLimit->setLatch();
  hacks.ppu.hiresMode7->setLatch();
  hacks.dsp.fast->setLatch();
  hacks.coprocessors.fast->setLatch();

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
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  controllerPort1.disconnect();
  controllerPort2.disconnect();
  expansionPort.disconnect();
  cartridge.unload();
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
  cartridge.power(reset);

  scheduler.primary(cpu);
}

auto Display::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Video::create("Display"));
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
  Node::load(node, from);
}

auto Speakers::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Audio::create("Speakers"));
  Node::load(node, from);
}

auto Hacks::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Settings::create("Hacks"));
  from = Node::load(node, from);

  node->append(ppu.fast = Node::Boolean::create("Fast PPU", false));
  Node::load(ppu.fast, from);

  node->append(ppu.noSpriteLimit = Node::Boolean::create("No Sprite Limit", false));
  Node::load(ppu.noSpriteLimit, from);

  node->append(ppu.hiresMode7 = Node::Boolean::create("Hires Mode 7", false));
  Node::load(ppu.hiresMode7, from);

  node->append(dsp.fast = Node::Boolean::create("Fast DSP", false));
  Node::load(dsp.fast, from);

  node->append(coprocessors.fast = Node::Boolean::create("Fast Coprocessors", false));
  Node::load(coprocessors.fast, from);
}

}
