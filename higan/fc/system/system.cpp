#include <fc/fc.hpp>

namespace higan::Famicom {

#include "serialization.cpp"
System system;
Scheduler scheduler;
Cheat cheat;

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(cartridge);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::load(Interface* interface) -> bool {
  this->interface = interface;
  information = {};

  auto document = BML::unserialize(interface->properties().serialize());
  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC-J") {
    information.region = Region::NTSCJ;
    information.frequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(cartridge.region() == "NTSC-U") {
    information.region = Region::NTSCU;
    information.frequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.frequency = Constants::Colorburst::PAL * 6.0;
  }

  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  if(!loaded()) return;
  cpu.peripherals.reset();
  controllerPort1.unload();
  controllerPort2.unload();
  cartridge.unload();
  information.loaded = false;
}

auto System::power(bool reset) -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power(reset);
  apu.power(reset);
  ppu.power(reset);
  scheduler.primary(cpu);

  controllerPort1.power(ID::Port::Controller1);
  controllerPort2.power(ID::Port::Controller2);

  controllerPort1.connect(option.port.controller1.device());
  controllerPort2.connect(option.port.controller2.device());
}

auto System::init() -> void {
  assert(interface != nullptr);
}

auto System::term() -> void {
}

}
