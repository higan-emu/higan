#include <ms/ms.hpp>

namespace higan::MasterSystem {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) {
    cpu.pollPause();
    vdp.refresh();
  }
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(vdp);
  scheduler.synchronize(psg);
}

auto System::load(Node::Object from) -> void {
  if(root) {
    save();
    unload();
  }

  root = Node::System::create(interface->name());
  root->load(from);

  regionNode = Node::String::create("Region", "NTSC");
  regionNode->allowedValues = {"NTSC", "PAL"};
  Node::load(regionNode, from);
  root->append(regionNode);

  controls.load(root, from);
  display.load(root, from);
  cartridge.load(root, from);
  if(!MasterSystem::Model::GameGear()) {
    controllerPort1.load(root, from);
    controllerPort2.load(root, from);
  }
}

auto System::save() -> void {
  cartridge.save();
}

auto System::unload() -> void {
  if(!MasterSystem::Model::GameGear()) {
    cpu.peripherals.reset();
    controllerPort1.disconnect();
    controllerPort2.disconnect();
  }
  cartridge.disconnect();
  root = {};
}

auto System::power() -> void {
  information = {};
  if(interface->name() == "ColecoVision" ) information.model = Model::ColecoVision;
  if(interface->name() == "SG-1000"      ) information.model = Model::SG1000;
  if(interface->name() == "SC-3000"      ) information.model = Model::SC3000;
  if(interface->name() == "Master System") information.model = Model::MasterSystem;
  if(interface->name() == "Game Gear"    ) information.model = Model::GameGear;
  for(auto& setting : root->find<Node::Setting>()) setting->setLatch();

  if(regionNode->latch() == "NTSC") {
    information.region = Region::NTSC;
    information.colorburst = Constants::Colorburst::NTSC;
  }

  if(regionNode->latch() == "PAL") {
    information.region = Region::PAL;
    information.colorburst = Constants::Colorburst::PAL * 4.0 / 5.0;
  }

  if(MasterSystem::Model::ColecoVision()) {
    if(auto fp = platform->open(root, "bios.rom", File::Read, File::Required)) {
      fp->read(bios, 0x2000);
    }
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
