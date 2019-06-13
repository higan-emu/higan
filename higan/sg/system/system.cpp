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
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "SG-1000") information.model = Model::SG1000;
  if(interface->name() == "SC-3000") information.model = Model::SC3000;

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC → PAL");
  regionNode->allowedValues = {
    "NTSC → PAL",
    "PAL → NTSC",
    "NTSC",
    "PAL"
  };
  Node::load(regionNode, from);
  node->append(regionNode);

  scheduler.reset();
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
  cartridge.port = {};
  controllerPort1.port = {};
  controllerPort2.port = {};
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  auto setRegion = [&](string region) {
    if(region == "NTSC") {
      information.region = Region::NTSC;
      information.colorburst = Constants::Colorburst::NTSC;
    }
    if(region == "PAL") {
      information.region = Region::PAL;
      information.colorburst = Constants::Colorburst::PAL * 4.0 / 5.0;
    }
  };
  auto regions = regionNode->latch().split("→").strip();
  setRegion(regions.first());
  for(auto& requested : reverse(regions)) {
    if(requested == cartridge.region()) setRegion(requested);
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, display.node->width, display.node->height);
  audio.reset(interface);

  cartridge.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.setPrimary(cpu);

  serializeInit();
}

}
