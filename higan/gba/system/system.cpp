#include <gba/gba.hpp>

namespace higan::GameBoyAdvance {

System system;
Scheduler scheduler;
#include "bios.cpp"
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
  if(GameBoyAdvance::Model::GameBoyPlayer()) player.frame();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(player);
}

auto System::load(Node::Object from) -> void {
  if(root) save(), unload();

  root = Node::System::create(interface->name());
  root->load(from);

  controls.load(root, from);
  display.load(root, from);
  cartridge.load(root, from);
}

auto System::unload() -> void {
  cartridge.disconnect();
  root = {};
}

auto System::save() -> void {
  cartridge.save();
}

auto System::power() -> void {
  for(auto& setting : root->find<Node::Setting>()) setting->setLatch();
  information = {};

  if(auto fp = platform->open(root, "bios.rom", File::Read, File::Required)) {
    fp->read(bios.data, bios.size);
  }

  serializeInit();

  video.reset(interface);
  display.screen = video.createScreen(display.node, 240, 160);
  audio.reset(interface);

  scheduler.reset();
  bus.power();
  player.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.primary(cpu);
}

}
