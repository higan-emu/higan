#include <pce/pce.hpp>

namespace higan::PCEngine {

System system;
Scheduler scheduler;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vce.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vce);
  scheduler.synchronize(vdc0);
  scheduler.synchronize(vdc1);
  scheduler.synchronize(psg);
}

auto System::load(Interface* interface, Model model) -> bool {
  this->interface = interface;
  information = {};
  information.model = model;

  auto document = BML::unserialize(interface->properties().serialize());
  if(!cartridge.load()) return false;

  cpu.load();
  serializeInit();
  information.colorburst = Constants::Colorburst::NTSC;
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
  cpu.save();
}

auto System::unload() -> void {
  cpu.peripherals.reset();
  controllerPort.unload();
  cartridge.unload();
}

auto System::power() -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  vce.power();
  vpc.power();
  vdc0.power();
  vdc1.power();
  psg.power();
  scheduler.primary(cpu);

  controllerPort.power();
  controllerPort.connect(option.port.controller.device());
}

}
