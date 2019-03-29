#include <cv/cv.hpp>

namespace higan::ColecoVision {

VDP vdp;
#include "serialization.cpp"

auto VDP::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::irq(bool line) -> void {
  cpu.setNMI(line);
}

auto VDP::frame() -> void {
  scheduler.exit(Scheduler::Event::Frame);
}

auto VDP::refresh() -> void {
  display.screen->refresh(buffer, 256 * sizeof(uint32), 256, 192);
}

auto VDP::power() -> void {
  TMS9918::power();
  Thread::create(system.colorburst() * 2, [&] {
    while(true) scheduler.synchronize(), main();
  });
  vram.allocate(0x4000);
}

}
