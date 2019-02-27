#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "display.cpp"
#include "speakers.cpp"
#include "hacks.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    ppu.refresh();

    auto reset = resetButton->value;
    platform->input(resetButton);
    if(!reset && resetButton->value) power(true);
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(smp);
  scheduler.synchronize(ppu);
  scheduler.synchronize(dsp);
  for(auto coprocessor : cpu.coprocessors) scheduler.synchronize(*coprocessor);
  for(auto peripheral : cpu.peripherals) scheduler.synchronize(*peripheral);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC");
  regionNode->allowedValues = {"NTSC", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  node->append(resetButton);

  information = {};
  if(regionNode->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.cpuFrequency = Constants::Colorburst::NTSC * 6.0;
  }
  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.cpuFrequency = Constants::Colorburst::PAL * 4.8;
  }

  hacks.load(node, from);
  bus.reset();
  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
  expansionPort.load(node, from);
  display.load(node, from);
  speakers.load(node, from);
  cpu.load(node, from);
  ppu.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  controllerPort1.port = {};
  controllerPort2.port = {};
  expansionPort.port = {};
  cartridge.port = {};
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power(bool reset) -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  video.reset(interface);
  display.screen = video.createScreen(display.node, 512, 480);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  scheduler.reset();
  cpu.power(reset);
  smp.power(reset);
  dsp.power(reset);
  ppu.power(reset);
  cartridge.power(reset);
  scheduler.primary(cpu);

  serializeInit();
}

}
