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
  if(node) unload();

  information = {};
  if(interface->name() == "Game Boy Advance") information.model = Model::GameBoyAdvance;
  if(interface->name() == "Game Boy Player" ) information.model = Model::GameBoyPlayer;

  node = Node::System::create(interface->name());
  node->load(from);

  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.disconnect();
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
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
