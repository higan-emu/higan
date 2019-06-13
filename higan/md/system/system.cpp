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
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};

  node = Node::System::create(interface->name());
  node->load(from);

  tmss = Node::Boolean::create("TMSS", false);
  Node::load(tmss, from);
  node->append(tmss);

  regionNode = Node::String::create("Region", "NTSC-J → NTSC-U → PAL");
  regionNode->allowedValues = {
    "NTSC-J → NTSC-U → PAL",
    "NTSC-U → NTSC-J → PAL",
    "PAL → NTSC-J → NTSC-U",
    "PAL → NTSC-U → NTSC-J",
    "NTSC-J",
    "NTSC-U",
    "PAL"
  };
  Node::load(regionNode, from);
  node->append(regionNode);

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  node->append(resetButton);

  scheduler.reset();
  display.load(node, from);
  cartridge.load(node, from);
  expansion.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
  extensionPort.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  expansion.port = {};
  controllerPort1.port = {};
  controllerPort2.port = {};
  extensionPort.port = {};
  mcd.unload();
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
  expansion.save();
}

auto System::power(bool reset) -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  auto setRegion = [&](string region) {
    if(region == "NTSC-J") {
      information.region = Region::NTSCJ;
      information.frequency = Constants::Colorburst::NTSC * 15.0;
    }
    if(region == "NTSC-U") {
      information.region = Region::NTSCU;
      information.frequency = Constants::Colorburst::NTSC * 15.0;
    }
    if(region == "PAL") {
      information.region = Region::PAL;
      information.frequency = Constants::Colorburst::PAL * 12.0;
    }
  };
  auto regions = regionNode->latch().split("→").strip();
  setRegion(regions.first());
  for(auto& requested : reverse(regions)) {
    for(auto& available : reverse(cartridge.regions())) {
      if(requested == available) setRegion(requested);
    }
  }
  information.megaCD = (bool)expansion.node;

  video.reset(interface);
  display.screen = video.createScreen(display.node, 1280, 480);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  cartridge.power();
  expansion.power();
  cpu.power(reset);
  apu.power(reset);
  vdp.power(reset);
  psg.power(reset);
  ym2612.power(reset);
  if(MegaCD()) mcd.power(reset);
  scheduler.setPrimary(cpu);

  serializeInit();
}

}
