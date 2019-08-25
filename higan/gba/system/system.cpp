#include <gba/gba.hpp>

namespace higan::GameBoyAdvance {

Scheduler scheduler;
System system;
#include "bios.cpp"
#include "controls.cpp"
#include "video.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Event::Frame) ppu.refresh();
  if(GameBoyAdvance::Model::GameBoyPlayer()) player.frame();
}

auto System::runToSave() -> void {
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Game Boy Advance") information.model = Model::GameBoyAdvance;
  if(interface->name() == "Game Boy Player" ) information.model = Model::GameBoyPlayer;

  higan::video.reset(interface);
  higan::audio.reset(interface);

  node = Node::System::create(interface->name());
  node->load(from);

  scheduler.reset();
  controls.load(node, from);
  video.load(node, from);
  ppu.load(node, from);
  apu.load(node, from);
  cartridge.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  ppu.unload();
  apu.unload();
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

  bus.power();
  player.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power(cpu);
}

}
