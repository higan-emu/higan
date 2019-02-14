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
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();

  auto reset = resetButton->value;
  platform->input(resetButton);
  if(!reset && resetButton->value) power(true);
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
  if(root) unload();

  root = Node::System::create("Super Famicom");
  root->load(from);

  regionSetting = Node::String::create("Region", "NTSC");
  regionSetting->allowedValues = {"NTSC", "PAL"};
  Node::load(regionSetting, from);
  root->append(regionSetting);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  root->append(resetButton);

  hacks.load(root, from);
  bus.reset();
  cartridge.load(root, from);
  controllerPort1.load(root, from);
  controllerPort2.load(root, from);
  expansionPort.load(root, from);
  display.load(root, from);
  speakers.load(root, from);
  cpu.load(root, from);
  ppu.load(root, from);
}

auto System::save() -> void {
  if(!root) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!root) return;
  save();
  controllerPort1.disconnect();
  controllerPort2.disconnect();
  expansionPort.disconnect();
  cartridge.disconnect();
  root = {};
}

auto System::power(bool reset) -> void {
  information = {};
  for(auto& setting : root->find<Node::Setting>()) {
    if(!setting->dynamic) setting->setLatch();
  }

  if(regionSetting->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.cpuFrequency = Constants::Colorburst::NTSC * 6.0;
  }

  if(regionSetting->latch() == "PAL") {
    information.region = Region::PAL;
    information.cpuFrequency = Constants::Colorburst::PAL * 4.8;
  }

  serializeInit();

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
}

}
