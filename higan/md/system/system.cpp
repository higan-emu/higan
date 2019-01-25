#include <md/md.hpp>

namespace higan::MegaDrive {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
  scheduler.synchronize(ym2612);
}

auto System::load(Interface* interface, maybe<Region> region) -> bool {
  this->interface = interface;
  information = {};

  auto document = BML::unserialize(interface->properties().serialize());
  auto system = document["system"];
  if(!cpu.load(system)) return false;
  if(!cartridge.load()) return false;

  if(cartridge.region() == "NTSC-J") {
    information.region = Region::NTSCJ;
    information.frequency = Constants::Colorburst::NTSC * 15.0;
  }
  if(cartridge.region() == "NTSC-U") {
    information.region = Region::NTSCU;
    information.frequency = Constants::Colorburst::NTSC * 15.0;
  }
  if(cartridge.region() == "PAL") {
    information.region = Region::PAL;
    information.frequency = Constants::Colorburst::PAL * 12.0;
  }

  serializeInit();
  return information.loaded = true;
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  cpu.peripherals.reset();
  controllerPort1.unload();
  controllerPort2.unload();
  extensionPort.unload();
  cartridge.unload();
}

auto System::power(bool reset) -> void {
  video.reset(interface);
  video.setPalette();
  audio.reset(interface);
  random.entropy(Random::Entropy::High);

  scheduler.reset();
  cartridge.power();
  cpu.power(reset);
  apu.power(reset);
  vdp.power(reset);
  psg.power(reset);
  ym2612.power(reset);
  scheduler.primary(cpu);

  controllerPort1.power(ID::Port::Controller1);
  controllerPort2.power(ID::Port::Controller2);
  extensionPort.power(ID::Port::Extension);

  controllerPort1.connect(option.port.controller1.device());
  controllerPort2.connect(option.port.controller2.device());
  extensionPort.connect(option.port.extension.device());
}

}
