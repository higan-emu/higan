#include <fc/fc.hpp>

namespace higan::Famicom {

System system;
Scheduler scheduler;
Cheat cheat;
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();

  auto reset = resetButton->value;
  platform->input(resetButton);
  if(!reset && resetButton->value) power(true);
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(cartridge);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC-J");
  regionNode->allowedValues = {"NTSC-J", "NTSC-U", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  node->append(resetButton);

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

  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
  display.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.disconnect();
  controllerPort1.disconnect();
  controllerPort2.disconnect();
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

  scheduler.reset();
  cartridge.power();
  cpu.power(reset);
  apu.power(reset);
  ppu.power(reset);
  scheduler.primary(cpu);

  serializeInit();
}

}
