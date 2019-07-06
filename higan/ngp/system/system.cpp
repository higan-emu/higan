#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

Scheduler scheduler;
System system;
#include "controls.cpp"
#include "video.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vpu.refresh();
}

auto System::runToSave() -> void {
  scheduler.enter(Scheduler::Mode::Serialize);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Neo Geo Pocket"      ) information.model = Model::NeoGeoPocket;
  if(interface->name() == "Neo Geo Pocket Color") information.model = Model::NeoGeoPocketColor;

  higan::video.reset(interface);
  higan::audio.reset(interface);

  node = Node::append<Node::System>(nullptr, from, interface->name());

  fastBoot = Node::append<Node::Boolean>(node, from, "Fast Boot", false);

  scheduler.reset();
  controls.load(node, from);
  video.load(node, from);
  vpu.load(node, from);
  cartridge.load(node, from);
}

auto System::unload() -> void {
  if(!node) return;
  bios.reset();
  cartridge.port = {};
  cpu.unload();
  apu.unload();
  vpu.unload();
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
  cpu.save();
  apu.save();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  bios.allocate(64_KiB);
  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
    bios.load(fp);
  }
  cpu.load();
  apu.load();

  cartridge.power();
  cpu.power();
  apu.power();
  vpu.power();
  psg.power();
  scheduler.setPrimary(cpu);

  if(fastBoot->latch() && cartridge.flash[0]) cpu.fastBoot();

  serializeInit();
}

}
