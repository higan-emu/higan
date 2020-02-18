#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "window.cpp"
#include "background.cpp"
#include "mode7.cpp"
#include "oam.cpp"
#include "object.cpp"
#include "dac.cpp"
#include "color.cpp"
#include "serialization.cpp"
#include "../ppu/counter/serialization.cpp"

auto PPU::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "PPU");
  from = Node::scan(parent = node, from);

  screen = Node::append<Node::Screen>(parent, from, "Screen");
  screen->colors(1 << 19, {&PPU::color, this});
  screen->setSize(512, 480);
  screen->setScale(0.5, 0.5);
  screen->setAspect(8.0, 7.0);
  from = Node::scan(parent = screen, from);

  region = Node::append<Node::String>(parent, from, "Region", "PAL", [&](auto region) {
    if(region == "NTSC") screen->setSize(512, 448);
    if(region == "PAL" ) screen->setSize(512, 480);
  });
  region->setAllowedValues({"NTSC", "PAL"});
  region->setDynamic(true);

  colorEmulation = Node::append<Node::Boolean>(parent, from, "Color Emulation", true, [&](auto value) {
    screen->resetPalette();
  });
  colorEmulation->setDynamic(true);

  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset
}

auto PPU::unload() -> void {
  node = {};
  screen = {};
  colorEmulation = {};

  output -= 16 * 512;
  delete[] output;
}

auto PPU::step(uint clocks) -> void {
  tick(clocks);
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto PPU::main() -> void {
  if(vcounter() < vdisp()) {
    step(512);
    dac.prepare();
    if(!io.displayDisable) {
      bg1.render();
      bg2.render();
      bg3.render();
      bg4.render();
      obj.render();
    }
    dac.render();
  }

  if(vcounter() == 240) {
    scheduler.exit(Event::Frame);
  }

  step(hperiod() - hcounter());
}

auto PPU::map() -> void {
  function<uint8 (uint24, uint8)> reader{&PPU::readIO, this};
  function<void (uint24, uint8)> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");
}

auto PPU::power(bool reset) -> void {
  Thread::create(system.cpuFrequency(), {&PPU::main, this});
  PPUcounter::reset();
}

auto PPU::refresh() -> void {
  auto data = output;
  bool hires = io.bgMode == 5 || io.bgMode == 6;
  bool phires = io.pseudoHires || io.bgMode == 5 || io.bgMode == 6;

  if(region->value() == "NTSC") {
    data += 2 * 512;
    if(overscan()) data += 16 * 512;
    uint width  = 256 << phires;
    uint height = 224 <<  hires;
    uint pitch  = 512 << !hires;
    screen->refresh(data, pitch * sizeof(uint32), width, height);
  }

  if(region->value() == "PAL") {
    if(!overscan()) data -= 14 * 512;
    uint width  = 256 << phires;
    uint height = 240 <<  hires;
    uint pitch  = 512 << !hires;
    screen->refresh(data, pitch * sizeof(uint32), width, height);
  }
}

}
