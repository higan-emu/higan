#include <gb/gb.hpp>

namespace higan::GameBoy {

#include "serialization.cpp"
System system;
Scheduler scheduler;
Cheat cheat;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(cartridge);
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::load(Interface* interface_, Model model_, maybe<uint> systemID) -> bool {
  interface = interface_;
  _model = model_;

  auto document = BML::unserialize(interface->properties().serialize());
  if(auto memory = document["system/memory(type=ROM,content=Boot)"]) {
    bootROM.allocate(memory["size"].natural());
    uint id = model() != Model::SuperGameBoy ? ID::System : systemID();
    string name = model() != Model::SuperGameBoy ? "boot.rom" : "lr35902.boot.rom";
    if(auto fp = platform->open(id, name, File::Read, File::Required)) {
      bootROM.load(fp);
    } else return false;
  } else return false;

  if(!cartridge.load()) return false;
  serializeInit();
  this->interface = interface;
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  bootROM.reset();
  _loaded = false;
}

auto System::power() -> void {
  if(model() != Model::SuperGameBoy) {
    video.reset(interface);
    video.setPalette();
    video.setEffect(Video::Effect::InterframeBlending, option.video.blurEmulation());
    audio.reset(interface);
  }

  scheduler.reset();
  bus.power();
  cartridge.power();
  cpu.power();
  ppu.power();
  apu.power();
  scheduler.primary(cpu);

  _clocksExecuted = 0;
}

}
