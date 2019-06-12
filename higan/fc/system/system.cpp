#include <fc/fc.hpp>

namespace higan::Famicom {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();

  auto reset = controls.reset->value;
  platform->input(controls.reset);
  if(!reset && controls.reset->value) power(true);
}

auto System::runToSave() -> void {
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC-J");
  regionNode->allowedValues = {"NTSC-J", "NTSC-U", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  information = {};
  if(regionNode->value() == "NTSC-J") {
    information.region = Region::NTSCJ;
    information.frequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(regionNode->value() == "NTSC-U") {
    information.region = Region::NTSCU;
    information.frequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(regionNode->value() == "PAL") {
    information.region = Region::PAL;
    information.frequency = Constants::Colorburst::PAL * 6.0;
  }

  scheduler.reset();
  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
  display.load(node, from);
  controls.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.unload();
  controllerPort1.port = {};
  controllerPort2.port = {};
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power(bool reset) -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  video.reset(interface);
  display.screen = video.createScreen(display.node, 256, 240);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  cartridge.power();
  cpu.power(reset);
  apu.power(reset);
  ppu.power(reset);
  scheduler.setPrimary(cpu);

  serializeInit();
}

}
