#include <pce/pce.hpp>

namespace higan::PCEngine {

System system;
Scheduler scheduler;
Cheat cheat;
#include "display.cpp"
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

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "PC Engine" ) information.model = Model::PCEngine;
  if(interface->name() == "SuperGrafx") information.model = Model::SuperGrafx;

  node = Node::System::create(interface->name());
  node->load(from);

  display.load(node, from);
  cartridge.load(node, from);
  controllerPort.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  controllerPort.port = {};
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  video.reset(interface);
  display.screen = video.createScreen(display.node, 1120, 240);
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

  serializeInit();
}

}
