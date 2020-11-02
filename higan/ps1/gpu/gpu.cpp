#include <ps1/ps1.hpp>

namespace higan::PlayStation {

GPU gpu;
#include "io.cpp"
#include "gp0.cpp"
#include "gp1.cpp"
#include "render.cpp"
#include "screen.cpp"
#include "serialization.cpp"

auto GPU::load(Node::Object parent) -> void {
  node = parent->append<Node::Component>("GPU");

  screen = node->append<Node::Screen>("Screen");
  setScreenColors(0);
//screen->setSize( 320, 240);
  screen->setSize( 640, 480);
//screen->setSize(1024, 512);

  vram.allocate(1_MiB);
}

auto GPU::unload() -> void {
  vram.reset();
  screen.reset();
  node.reset();
}

auto GPU::main() -> void {
  if(io.vcounter <= 240) {
    timer.hsync(1);
    timer.hsync(0);
  }

  if(io.vcounter == 240) {
    timer.vsync(1);
    interrupt.raise(Interrupt::Vblank);
  }

  if(++io.vcounter == 262) {
    timer.vsync(0);
    io.vcounter = 0;
    io.field = !io.field;
    interrupt.lower(Interrupt::Vblank);
    refreshed = true;
  }

  step(33'868'800 / 60 / 262);
}

auto GPU::step(uint clocks) -> void {
  Thread::clock += clocks;
}

auto GPU::refresh() -> void {
  u32 bytesPerPixel = refreshOutput();
  u32 pitch = 1024 * 2 / bytesPerPixel;
//screen->refresh((uint32*)output, pitch * sizeof(uint32),  320, 240);
  screen->refresh((uint32*)output, pitch * sizeof(uint32),  640, 480);
//screen->refresh((uint32*)output, pitch * sizeof(uint32), 1024, 512);
}

auto GPU::power(bool reset) -> void {
  Thread::reset();
  refreshed = false;
  vram.fill();
  io = {};
}

}
