#include <pce/pce.hpp>

namespace higan::PCEngine {

VDP vdp;
#include "vce.cpp"
#include "vdc.cpp"
#include "vpc.cpp"
#include "irq.cpp"
#include "dma.cpp"
#include "background.cpp"
#include "sprite.cpp"
#include "color.cpp"
#include "serialization.cpp"

auto VDP::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "VDP");
  from = Node::scan(parent = node, from);

  screen = Node::append<Node::Screen>(parent, from, "Screen");
  screen->colors(1 << 9, {&VDP::color, this});
  screen->setSize(1024, 240);
  screen->setScale(0.25, 1.0);
  screen->setAspect(8.0, 7.0);
  from = Node::scan(parent = screen, from);
}

auto VDP::unload() -> void {
  node = {};
  screen = {};
}

auto VDP::main() -> void {
  vdc0.prologue(io.vcounter); if(Model::SuperGrafx())
  vdc1.prologue(io.vcounter);

  if(io.vcounter == 0) width256 = 0, width344 = 0, width512 = 0;

  if(io.vcounter >= 13 && io.vcounter < 240 + 13 && !runAhead()) {
    step(512);

    switch(vce.width()) {
    case 256: widths[io.vcounter] = 256; width256 = 1; break;
    case 344: widths[io.vcounter] = 344; width344 = 1; break;
    case 512: widths[io.vcounter] = 512; width512 = 1; break;
    }

    vdc0.render(io.vcounter); if(Model::SuperGrafx())
    vdc1.render(io.vcounter); if(Model::SuperGrafx())
    vpc.render();

    auto output = buffer + 1365 * io.vcounter;

    if(Model::PCEngine()) {
      for(uint x : range(vce.width())) {
        output[x] = vce.cram.read(vdc0.output[x]);
      }
    }

    if(Model::SuperGrafx()) {
      for(uint x : range(vce.width())) {
        output[x] = vce.cram.read(vpc.output[x]);
      }
    }
  }

  step(1365 - io.hcounter);

  vdc0.epilogue(io.vcounter); if(Model::SuperGrafx())
  vdc1.epilogue(io.vcounter);

  io.hcounter = 0;
  io.vcounter++;
  if(io.vcounter == 240) scheduler.exit(Event::Frame);
  if(io.vcounter == 262) io.vcounter = 0;
}

auto VDP::step(uint clocks) -> void {
  io.hcounter += clocks;
  vdc0.dma.step(clocks); if(Model::SuperGrafx())
  vdc1.dma.step(clocks);

  Thread::step(clocks);
  synchronize(cpu);
}

auto VDP::refresh() -> void {
  if(width256 == 1 && width344 == 0 && width512 == 0) {
    return screen->refresh(buffer + 1365 * 13, 1365 * sizeof(uint32), 256, 240);
  }

  if(width256 == 0 && width344 == 1 && width512 == 0) {
    return screen->refresh(buffer + 1365 * 13, 1365 * sizeof(uint32), 344, 240);
  }

  if(width256 == 0 && width344 == 0 && width512 == 1) {
    return screen->refresh(buffer + 1365 * 13, 1365 * sizeof(uint32), 512, 240);
  }

  //this frame contains mixed resolutions: normalize every scanline to 1024-width
  for(uint y = 13; y < 240 + 13; y++) {
    auto output = buffer + 1365 * y;
    switch(widths[y]) {

    case 256: {
      auto source = &output[256];
      auto target = &output[256 * 4];
      for(uint x : range(256)) {
        auto color = *--source;
        *--target = color;
        *--target = color;
        *--target = color;
        *--target = color;
      }
    } break;

    case 344: {
      auto source = &output[344];
      auto target = &output[344 * 3];
      for(uint x : range(344)) {
        auto color = *--source;
        *--target = color;
        *--target = color;
        *--target = color;
      }
    } break;

    case 512: {
      auto source = &output[512];
      auto target = &output[512 * 2];
      for(uint x : range(512)) {
        auto color = *--source;
        *--target = color;
        *--target = color;
      }
    } break;

    }
  }

  return screen->refresh(buffer + 1365 * 13, 1365 * sizeof(uint32), 1024, 240);
}

auto VDP::power() -> void {
  Thread::create(system.colorburst() * 6.0, {&VDP::main, this});

  vce.power();
  vdc0.power(); if(Model::SuperGrafx())
  vdc1.power(); if(Model::SuperGrafx())
  vpc.power();
}

}
