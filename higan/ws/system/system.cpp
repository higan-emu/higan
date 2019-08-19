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
  information.abstract = !(bool)from;

  if(interface->name() == "WonderSwan"         ) information.model = Model::WonderSwan;
  if(interface->name() == "WonderSwan Color"   ) information.model = Model::WonderSwanColor;
  if(interface->name() == "SwanCrystal"        ) information.model = Model::SwanCrystal;
  if(interface->name() == "Pocket Challenge V2") information.model = Model::PocketChallengeV2;
  if(interface->name() == "mamaMitte"          ) information.model = Model::MamaMitte;

  higan::video.reset(interface);
  higan::audio.reset(interface);

  node = Node::append<Node::System>(nullptr, from, interface->name());

  if(WonderSwan::Model::WonderSwan()) {
    if(auto fp = platform->open(node, "boot.rom", File::Read)) {
      bootROM.allocate(4_KiB);
      bootROM.load(fp);
    }
    eeprom.allocate(128, 16);
  }

  if(WonderSwan::Model::WonderSwanColor()) {
    if(auto fp = platform->open(node, "boot.rom", File::Read)) {
      bootROM.allocate(8_KiB);
      bootROM.load(fp);
    }
    eeprom.allocate(2048, 16);
  }

  if(WonderSwan::Model::SwanCrystal()) {
    if(auto fp = platform->open(node, "boot.rom", File::Read)) {
      bootROM.allocate(8_KiB);
      bootROM.load(fp);
    }
    eeprom.allocate(2048, 16);
  }

  if(WonderSwan::Model::PocketChallengeV2()) {
    if(auto fp = platform->open(node, "boot.rom", File::Read)) {
      bootROM.allocate(4_KiB);
      bootROM.load(fp);
    }
    eeprom.allocate(128, 16);
  }

  if(WonderSwan::Model::MamaMitte()) {
    if(auto fp = platform->open(node, "boot.rom", File::Read)) {
      bootROM.allocate(8_KiB);
      bootROM.load(fp);
    }
    eeprom.allocate(2048, 16);
  }

  if(!abstract()) {
    if(auto fp = platform->open(node, "save.eeprom", File::Read)) {
      fp->read(eeprom.data, eeprom.size);
    }
  }

  scheduler.reset();
  controls.load(node, from);
  video.load(node, from);
  ppu.load(node, from);
  cartridge.load(node, from);
}

auto System::save() -> void {
  if(!node) return;

  if(!abstract()) {
    if(auto fp = platform->open(node, "save.eeprom", File::Write)) {
      fp->write(eeprom.data, eeprom.size);
    }
  }

  cartridge.save();
}

auto System::unload() -> void {
  if(!node) return;

  save();
  bootROM.reset();
  eeprom.reset();
  cartridge.port = {};
  ppu.unload();
  node = {};
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

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
