#include <msx/msx.hpp>

namespace higan::MSX {

VDP vdp;
#include "serialization.cpp"

auto VDP::load(Node::Object parent, Node::Object from) -> void {
  display.create(system.video.node);
}

auto VDP::unload() -> void {
  display.destroy();
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto VDP::irq(bool line) -> void {
  cpu.setIRQ(line);
}

auto VDP::frame() -> void {
  scheduler.exit(Event::Frame);
}

auto VDP::refresh() -> void {
  if(Model::MSX()) {
    display.refresh(TMS9918::buffer, 256 * sizeof(uint32), 256, 192);
  }

  if(Model::MSX2()) {
    display.refresh(V9938::buffer, 512 * sizeof(uint32), 512, 424);
  }
}

auto VDP::power() -> void {
  if(Model::MSX()) {
    TMS9918::vram.allocate(16_KiB);
    TMS9918::power();
    Thread::create(system.colorburst() * 2, [&] { TMS9918::main(); });
  }

  if(Model::MSX2()) {
    V9938::videoRAM.allocate(128_KiB);
    V9938::expansionRAM.allocate(64_KiB);
    V9938::power();
    Thread::create(system.colorburst() * 2, [&] { V9938::main(); });
  }
}

/* Z80 I/O ports 0x98 - 0x9b */

auto VDP::read(uint2 port) -> uint8 {
  if(Model::MSX())
  switch(port) {
  case 0: return TMS9918::data();
  case 1: return TMS9918::status();
  }

  if(Model::MSX2())
  switch(port) {
  case 0: return V9938::data();
  case 1: return V9938::status();
  }

  return 0xff;
}

auto VDP::write(uint2 port, uint8 data) -> void {
  if(Model::MSX())
  switch(port) {
  case 0: return TMS9918::data(data);
  case 1: return TMS9918::control(data);
  }

  if(Model::MSX2())
  switch(port) {
  case 0: return V9938::data(data);
  case 1: return V9938::control(data);
  case 2: return V9938::palette(data);
  case 3: return V9938::register(data);
  }
}

}
