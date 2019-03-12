#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

VPU vpu;
#include "memory.cpp"
#include "window.cpp"
#include "plane.cpp"
#include "sprite.cpp"
#include "serialization.cpp"

auto VPU::main() -> void {
  if(io.vcounter < 152) {
    auto output = buffer + io.vcounter * 160;
    for(uint hclock : range(160)) {
      uint12 color;
      if(Model::NeoGeoPocketColor() && background.mode == 2) {
        color = colorPalette[0xf0 + background.color];
      }
      if(!io.planePriority) {
        if(auto output = renderPlane(plane2)) color = *output;
        if(auto output = renderPlane(plane1)) color = *output;
      } else {
        if(auto output = renderPlane(plane1)) color = *output;
        if(auto output = renderPlane(plane2)) color = *output;
      }
      if(auto output = renderSprite()) color = *output;
      if(auto output = renderWindow()) color = *output;

      output[hclock] = color;
      io.hcounter++;
      step(3);
    }
    if(io.vcounter <= 150) {
      io.hblankActive = 1;
      cpu.setPin33(0);
    }
  }
  while(io.hcounter < 171) {
    io.hcounter++;
    step(3);
  }

  io.hcounter = 0;
  io.hblankActive = 0;
  cpu.setPin33(1);

  io.vcounter++;
  if(io.vcounter == 152) {
    io.vblankActive = 1;
    cpu.setPin36(0);
    scheduler.exit(Scheduler::Event::Frame);
  }
  if(io.vcounter == io.vlines) {
    io.hblankActive = 1;
    cpu.setPin33(0);
  }
  if(io.vcounter > io.vlines) {
    io.vcounter = 0;
    io.vblankActive = 0;
    io.characterOver = 0;
    cpu.setPin36(1);
  }

  //note: this is not the most intuitive place to call this,
  //but calling after every CPU instruction is too demanding
  cpu.pollPowerButton();
}

auto VPU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
}

auto VPU::refresh() -> void {
  display.screen->refresh(buffer, 160 * sizeof(uint32), 160, 152);
}

auto VPU::power() -> void {
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  scrollRAM.allocate(0x1000);
  characterRAM.allocate(0x2000);
  for(auto& p : colorPalette) p = {};
  background = {};
  window = {};
  plane1 = {0x000, 0x40, 0xd0};
  plane2 = {0x800, 0x80, 0xe0};
  sprite = {0x00, 0xc0};
  for(auto& s : sprites) s = {};
  led = {};
  screen = {};
  io = {};
}

}
