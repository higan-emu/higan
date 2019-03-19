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
      bool validPlane1 = renderPlane(plane1);
      bool validPlane2 = renderPlane(plane2);
      bool validSprite = renderSprite();
      bool validWindow = renderWindow();

      uint12 color;
      if(Model::NeoGeoPocketColor() && background.mode == 2) {
        color = colorPalette[0xf0 + background.color];
      }

      if(validSprite && sprite.priority == 1) color = sprite.output;
      if(validPlane1 && plane1.priority == 0) color = plane1.output;
      if(validPlane2 && plane2.priority == 0) color = plane2.output;
      if(validSprite && sprite.priority == 2) color = sprite.output;
      if(validPlane1 && plane1.priority == 1) color = plane1.output;
      if(validPlane2 && plane2.priority == 1) color = plane2.output;
      if(validSprite && sprite.priority == 3) color = sprite.output;
      if(validWindow) color = window.output;

      if(Model::NeoGeoPocket() && screen.negate) {
        color ^= 7;
      }

      output[hclock] = color;
      io.hcounter++;
      step(3);
    }
    if(io.vcounter <= 150) {
      io.hblankActive = 1;
      cpu.ti0 = !io.hblankEnableIRQ;
    }
  }
  while(io.hcounter < 171) {
    io.hcounter++;
    step(3);
  }

  io.hcounter = 0;
  io.hblankActive = 0;
  cpu.ti0 = 1;

  io.vcounter++;
  if(io.vcounter == 152) {
    io.vblankActive = 1;
    cpu.int4.set(!io.vblankEnableIRQ);
    scheduler.exit(Scheduler::Event::Frame);
  }
  if(io.vcounter == io.vlines) {
    io.hblankActive = 1;
    cpu.ti0 = !io.hblankEnableIRQ;
  }
  if(io.vcounter > io.vlines) {
    io.vcounter = 0;
    io.vblankActive = 0;
    io.characterOver = 0;
    cpu.int4.set(1);
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
