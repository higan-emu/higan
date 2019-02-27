#include <sg/sg.hpp>

namespace higan::SG1000 {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    vdp.refresh();
    controls.poll();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "SG-1000") information.model = Model::SG1000;
  if(interface->name() == "SC-3000") information.model = Model::SC3000;

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC");
  regionNode->allowedValues = {"NTSC", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.disconnect();
  controllerPort1.disconnect();
  controllerPort2.disconnect();
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  if(regionNode->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Constants::Colorburst::NTSC;
  }

  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Constants::Colorburst::PAL * 4.0 / 5.0;
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, display.node->width, display.node->height);
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.primary(cpu);

  serializeInit();
}

}
