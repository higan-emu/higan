#include <msx/msx.hpp>

namespace higan::MSX {

System system;
Scheduler scheduler;
Cheat cheat;
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vdp.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "MSX"  ) information.model = Model::MSX;
  if(interface->name() == "MSX2" ) information.model = Model::MSX2;
  if(interface->name() == "MSX2+") information.model = Model::MSX2Plus;

  node = Node::System::create(interface->name());
  node->load(from);

  regionNode = Node::String::create("Region", "NTSC");
  regionNode->allowedValues = {"NTSC", "PAL"};
  Node::load(regionNode, from);
  node->append(regionNode);

  scheduler.reset();
  display.load(node, from);
  keyboard.load(node, from);
  cartridge.load(node, from);
  expansion.load(node, from);
  controllerPort1.load(node, from);
  controllerPort2.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  save();
  cartridge.port = {};
  expansion.port = {};
  controllerPort1.port = {};
  controllerPort2.port = {};
  node = {};
  bios.reset();
  sub.reset();
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
  expansion.save();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  if(regionNode->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Constants::Colorburst::NTSC;
  }
  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Constants::Colorburst::PAL;
  }

  bios.allocate(32_KiB);
  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
    bios.load(fp);
  }

  if(model() != Model::MSX) {
    sub.allocate(16_KiB);
    if(auto fp = platform->open(node, "sub.rom", File::Read, File::Required)) {
      sub.load(fp);
    }
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, display.node->width, display.node->height);
  audio.reset(interface);

  keyboard.power();
  cartridge.power();
  expansion.power();
  cpu.power();
  vdp.power();
  psg.power();
  scheduler.primary(cpu);

  serializeInit();
}

}
