#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

System system;
Scheduler scheduler;
Cheat cheat;
#include "controls.cpp"
#include "display.cpp"
#include "serialization.cpp"

auto System::run() -> void {
  if(scheduler.enter() == Scheduler::Event::Frame) vpu.refresh();
}

auto System::runToSave() -> void {
  scheduler.synchronize(cpu);
  scheduler.synchronize(apu);
  scheduler.synchronize(vpu);
  scheduler.synchronize(psg);
}

auto System::load(Node::Object from) -> void {
  if(node) unload();

  information = {};
  if(interface->name() == "Neo Geo Pocket"      ) information.model = Model::NeoGeoPocket;
  if(interface->name() == "Neo Geo Pocket Color") information.model = Model::NeoGeoPocketColor;

  node = Node::System::create(interface->name());
  node->load(from);

  controls.load(node, from);
  display.load(node, from);
  cartridge.load(node, from);
  cpu.load();
}

auto System::unload() -> void {
  if(!node) return;
  bios.reset();
  cartridge.port = {};
  cpu.unload();
  node = {};
}

auto System::save() -> void {
  if(!node) return;
  cartridge.save();
  cpu.save();
}

auto System::power() -> void {
  for(auto& setting : node->find<Node::Setting>()) setting->setLatch();

  bios.allocate(64_KiB);
  if(auto fp = platform->open(node, "bios.rom", File::Read, File::Required)) {
    bios.load(fp);
  }

  video.reset(interface);
  display.screen = video.createScreen(display.node, 160, 152);
  audio.reset(interface);

  scheduler.reset();
  cartridge.power();
  cpu.power();
  apu.power();
  vpu.power();
  psg.power();
  scheduler.primary(cpu);

  serializeInit();
}

}
