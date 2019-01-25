#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    vpu.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vpu);
  scheduler.synchronize(psg);
}

auto System::load(Interface* interface, Model model_) -> bool {
  this->interface = interface;
  information.model = model_;

  auto document = BML::unserialize(interface->properties().serialize());
  if(auto memory = document["system/memory(type=ROM,content=BIOS)"]) {
    bios.allocate(memory["size"].natural());
    if(auto fp = platform->open(ID::System, "bios.rom", File::Read, File::Required)) {
      bios.load(fp);
    } else return false;
  } else return false;

  if(model() == Model::NeoGeoPocket) {
  }

  if(model() == Model::NeoGeoPocketColor) {
  }

  if(!cartridge.load()) return false;
  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  if(!loaded()) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  bios.reset();
  cartridge.unload();
  information.loaded = false;
}

auto System::power() -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  apu.power();
  vpu.power();
  psg.power();
  scheduler.primary(cpu);
}

}
