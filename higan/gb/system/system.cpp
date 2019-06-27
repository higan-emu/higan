#include <gb/gb.hpp>

namespace higan::GameBoy {

Cheat cheat;
Scheduler scheduler;
System system;
#include "controls.cpp"
#include "video.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Game Boy"      ) information.model = Model::GameBoy;
  if(interface->name() == "Game Boy Color") information.model = Model::GameBoyColor;

  if(!GameBoy::Model::SuperGameBoy()) {
    ::higan::video.reset(interface);  //todo: no :: prefix
    ::higan::audio.reset(interface);  //todo: no :: prefix
  }

  node = Node::append<Node::System>(nullptr, from, interface->name());

  scheduler.reset();
  controls.load(node, from);
  video.load(node, from);
  cpu.load(node, from);
  ppu.load(node, from);
  cartridge.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  ppu.unload();
  bootROM.reset();
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  string name = "boot.rom";  //fallback name (should not be used)
  if(GameBoy::Model::GameBoy()) {
    bootROM.allocate(256);
    if(cpu.version->latch() == "DMG-CPU"  ) name = "boot.dmg-0.rom";
    if(cpu.version->latch() == "DMG-CPU A") name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "DMG-CPU B") name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "DMG-CPU C") name = "boot.dmg-1.rom";
    if(cpu.version->latch() == "CPU MGB"  ) name = "boot.mgb.rom";
  }
  if(GameBoy::Model::SuperGameBoy()) {
    bootROM.allocate(256);
    name = "sm83.boot.rom";  //filename is identical for SGB1 and SGB2 boot ROM
  }
  if(GameBoy::Model::GameBoyColor()) {
    bootROM.allocate(2048);
    name = "boot.rom";  //only one known CGB boot ROM
  }
  if(auto fp = platform->open(node, name, File::Read, File::Required)) {
    bootROM.load(fp);
  }

  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.setPrimary(cpu);

  serializeInit();
}

}
