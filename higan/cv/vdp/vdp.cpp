#include <cv/cv.hpp>

namespace higan::ColecoVision {

VDP vdp;
#include "serialization.cpp"

auto VDP::load(Node::Object parent, Node::Object from) -> void {
  screen = video.createScreen(system.video.node);
}

auto VDP::unload() -> void {
  screen = {};
}

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto VDP::irq(bool line) -> void {
  cpu.setNMI(line);
}

auto VDP::frame() -> void {
  scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::refresh() -> void {
  screen->refresh(buffer, 256 * sizeof(uint32), 256, 192);
}

auto VDP::power() -> void {
  TMS9918::power();
  Thread::create(system.colorburst() * 2, [&] { main(); });
  vram.allocate(0x4000);
}

}
