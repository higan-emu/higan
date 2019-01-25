#include <msx/msx.hpp>

namespace higan::MSX {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    vdp.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Interface* interface, Model model) -> bool {
  this->interface = interface;
  information = {};
  information.model = model;

  auto document = BML::unserialize(interface->properties().serialize());

  if(auto memory = document["system/memory(type=ROM,content=BIOS)"]) {
    bios.allocate(memory["size"].natural());
    if(auto fp = platform->open(ID::System, "bios.rom", File::Read, File::Required)) {
      bios.load(fp);
    } else return false;
  } else return false;

  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Constants::Colorburst::NTSC;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Constants::Colorburst::PAL;
  }

  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  cartridge.unload();
}

auto System::power() -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.primary(cpu);
}

}
