#include <ws/ws.hpp>

namespace higan::WonderSwan {

Scheduler scheduler;
System system;
#include "controls.cpp"
#include "video.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) ppu.refresh();
  controls.poll();
}

auto System::runToSave() -> void {
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "WonderSwan"         ) information.model = Model::WonderSwan;
  if(interface->name() == "WonderSwan Color"   ) information.model = Model::WonderSwanColor;
  if(interface->name() == "SwanCrystal"        ) information.model = Model::SwanCrystal;
  if(interface->name() == "Pocket Challenge V2") information.model = Model::PocketChallengeV2;

  higan::video.reset(interface);
  higan::audio.reset(interface);

  node = Node::append<Node::System>(nullptr, from, interface->name());

  scheduler.reset();
  controls.load(node, from);
  video.load(node, from);
  ppu.load(node, from);
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
  ppu.unload();
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

  bus.power();
  iram.power();
  eeprom.power();
  cpu.power();
  ppu.power();
  apu.power();
  cartridge.power();
  scheduler.power(cpu);

  serializeInit();

  bus.map(this, 0x0060);
  bus.map(this, 0x00ba, 0x00be);

  r.unknown = 0;
  r.format = 0;
  r.depth = 0;
  r.color = 0;
}

}
