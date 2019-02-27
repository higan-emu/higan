#include <ws/ws.hpp>

namespace higan::WonderSwan {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
  controls.poll();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(ppu);
  scheduler.synchronize(apu);
  scheduler.synchronize(cartridge);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "WonderSwan"         ) information.model = Model::WonderSwan;
  if(interface->name() == "WonderSwan Color"   ) information.model = Model::WonderSwanColor;
  if(interface->name() == "SwanCrystal"        ) information.model = Model::SwanCrystal;
  if(interface->name() == "Pocket Challenge V2") information.model = Model::PocketChallengeV2;

  node = Node::System::create(interface->name());
  node->load(from);

  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;
  save();
  eeprom.setSize(0);
  cartridge.port = {};
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  if(WonderSwan::Model::WonderSwan() || WonderSwan::Model::PocketChallengeV2()) {
    eeprom.setSize(128);
  }
  if(WonderSwan::Model::WonderSwanColor() || WonderSwan::Model::SwanCrystal()) {
    eeprom.setSize(2048);
  }
  eeprom.erase();
  //initialize user-data section
  for(uint address = 0x0030; address <= 0x003a; address++) eeprom[address] = 0x0000;
  if(auto fp = platform->open(node, "save.eeprom", File::Read)) {
    fp->read(eeprom.data(), eeprom.size());
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, 224, 144);
  audio.reset(interface);

  scheduler.reset();
  bus.power();
  iram.power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.primary(cpu);

  serializeInit();

  bus.map(this, 0x0060);
  bus.map(this, 0x00ba, 0x00be);

  r.unknown = 0;
  r.format = 0;
  r.depth = 0;
  r.color = 0;
}

}
