#include <gb/gb.hpp>

namespace higan::GameBoy {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(cartridge);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Game Boy"      ) information.model = Model::GameBoy;
  if(interface->name() == "Game Boy Color") information.model = Model::GameBoyColor;

  node = Node::System::create(interface->name());
  node->load(from);

  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.disconnect();
  bootROM.reset();
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  bootROM.allocate(!GameBoy::Model::GameBoyColor() ? 256 : 2048);
  string name = !GameBoy::Model::SuperGameBoy() ? "boot.rom" : "lr35902.boot.rom";
  if(auto fp = platform->open(node, name, File::Read, File::Required)) {
    bootROM.load(fp);
  } else return;

  if(!GameBoy::Model::SuperGameBoy()) {
    video.reset(interface);
    display.screen = video.createScreen(display.node, 160, 144);
    audio.reset(interface);
  }

  scheduler.reset();
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.primary(cpu);

  serializeInit();
}

}
