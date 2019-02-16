#include <pce/pce.hpp>

namespace higan::PCEngine {

VCE vce;
#include "memory.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto VCE::main() -> void {
  if(timing.vclock == 0) {
    vdc0.frame();
    vdc1.frame();
  }

  vdc0.scanline();
  vdc1.scanline();
  timing.hclock = 0;

  auto output = buffer + 1365 * timing.vclock;

  while(timing.hclock < 1360) {
    uint9 color;
    if(Model::PCEngine()) color = vdc0.bus();
    if(Model::SuperGrafx()) color = vpc.bus(timing.hclock);
    color = cram.read(color);

    if(clock() >= 2) *output++ = color;
    if(clock() >= 2) *output++ = color;
    if(clock() >= 3) *output++ = color;
    if(clock() >= 4) *output++ = color;
    step(clock());
  }

  step(1365 - timing.hclock);

  if(++timing.vclock == 262) {
    timing.vclock = 0;
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto VCE::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(vdc0);
  synchronize(vdc1);

  timing.hclock += clocks;
}

auto VCE::refresh() -> void {
  display.screen->refresh(buffer + 1365 * 13, 1365 * sizeof(uint32), 1120, 240);
}

auto VCE::power() -> void {
  Thread::create(system.colorburst() * 6.0, [&] {
    while(true) scheduler.synchronize(), main();
  });

  for(auto& pixel : buffer) pixel = 0;
  cram = {};
  timing = {};
  io = {};
}

}
