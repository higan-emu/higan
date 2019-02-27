#include <md/md.hpp>

namespace higan::MegaDrive {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();

  auto reset = resetButton->value;
  platform->input(resetButton);
  if(!reset && resetButton->value) power(true);
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
  scheduler.synchronize(ym2612);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  node = Node::System::create(interface->name());
  node->load(from);

  tmss = Node::Boolean::create("TMSS", false);
  Node::load(tmss, from);
  node->append(tmss);

  regionNode = Node::String::create("Region", "NTSC-J");
  regionNode->allowedValues = {"NTSC-J", "NTSC-U", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  node->append(resetButton);

  display.load(node, from);
  cartridge.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
  extensionPort.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  controllerPort1.port = {};
  controllerPort2.port = {};
  extensionPort.port = {};
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power(bool reset) -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  information = {};
  if(regionNode->latch() == "NTSC-J") {
    information.region = Region::NTSCJ;
    information.frequency = Constants::Colorburst::NTSC * 15.0;
  }
  if(regionNode->latch() == "NTSC-U") {
    information.region = Region::NTSCU;
    information.frequency = Constants::Colorburst::NTSC * 15.0;
  }
  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.frequency = Constants::Colorburst::PAL * 12.0;
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, 1280, 480);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  scheduler.reset();
  cartridge.power();
  cpu.power(reset);
  apu.power(reset);
  vdp.power(reset);
  psg.power(reset);
  ym2612.power(reset);
  scheduler.primary(cpu);

  serializeInit();
}

}
