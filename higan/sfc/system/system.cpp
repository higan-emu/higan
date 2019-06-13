#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

System system;
Scheduler scheduler;
Random random;
Cheat cheat;
#include "display.cpp"
#include "speakers.cpp"
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
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};

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

  resetButton = Node::Button::create("Reset");
  Node::load(resetButton, from);
  node->append(resetButton);

  scheduler.reset();
  bus.reset();
  cartridgePort.load(node, from);
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
  cartridgePort.port.reset();
  controllerPort1.port.reset();
  controllerPort2.port.reset();
  expansionPort.port.reset();
  ppu.unload();
  node.reset();
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::power(bool reset) -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  auto setRegion = [&](string region) {
    if(region == "NTSC") {
      information.region = Region::NTSC;
      information.cpuFrequency = Constants::Colorburst::NTSC * 6.0;
    }
    if(region == "PAL") {
      information.region = Region::PAL;
      information.cpuFrequency = Constants::Colorburst::PAL * 4.8;
    }
  };
  auto regions = regionNode->latch().split("→").strip();
  setRegion(regions.first());
  for(auto& requested : reverse(regions)) {
    if(requested == cartridge.region()) setRegion(requested);
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, 512, 480);
  audio.reset(interface);
  random.entropy(Random::Entropy::Low);

  cpu.power(reset);
  smp.power(reset);
  dsp.power(reset);
  ppu.power(reset);
  cartridge.power(reset);
  scheduler.setPrimary(cpu);

  serializeInit();
}

}
