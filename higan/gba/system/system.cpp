#include <gba/gba.hpp>

namespace higan::GameBoyAdvance {

System system;
Scheduler scheduler;
#include "bios.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(player);
}

auto System::power() -> void {
  video.reset(interface);
  video.setPalette();
  video.setEffect(Video::Effect::InterframeBlending, option.video.interframeBlending());
  video.setEffect(Video::Effect::RotateLeft, option.video.rotateLeft());
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

auto System::initialize(Interface* interface) -> void {
  this->interface = interface;
  object = {};
  object.id = 0;
  object.type = "System";
  object.name = "Game Boy Advance";
}

auto System::load() -> bool {
  information = {};

  auto document = BML::unserialize(interface->properties().serialize());

  if(auto name = document["system/memory(type=ROM,content=BIOS)"]) {
    if(auto fp = platform->open(ID::System, "bios.rom", File::Read, File::Required)) {
      fp->read(bios.data, bios.size);
    } else return false;
  } else return false;

//if(!cartridge.load()) return false;

  serializeInit();
  return _loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  cartridge.unload();
  _loaded = false;
}

}
