#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "window.cpp"
#include "mosaic.cpp"
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
  if(vcounter() == 0) {
    state.interlace = io.interlace;
    state.overscan = io.overscan;
    obj.io.rangeOver = 0;
    obj.io.timeOver = 0;
    width256 = 0;
    width512 = 0;
  }

  if(vcounter() < vdisp()) {
    uint width = hires() ? 512 : 256;
    if(width == 256) width256 = 1;
    if(width == 512) width512 = 1;
    widths[vcounter()] = width;

    step(512);
    mosaic.scanline();
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

  if(vcounter() == vdisp()) {
    if(!io.displayDisable) obj.addressReset();
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

  memory::fill<uint32>(output, 512 * 480);

  for(auto& word : vram.data) word = 0;

  state = {};
  latch = {};
  io = {};
  mode7 = {};
  window.io = {};
  mosaic.power();
  bg1.io = {};
  bg1.window = {};
  bg2.io = {};
  bg3.window = {};
  bg3.io = {};
  bg3.window = {};
  bg4.io = {};
  bg4.window = {};
  for(auto& object : obj.oam.objects) object = {};
  obj.io = {};
  obj.window = {};
  for(auto& item : obj.items) item = {};
  for(auto& tile : obj.tiles) tile = {};
  for(auto& color : dac.cgram) color = 0;
  dac.io = {};
  dac.window = {};

  updateVideoMode();
}

auto PPU::refresh() -> void {
  auto data  = output;
  uint width = width512 ? 512 : 256;
  uint pitch = state.interlace ? 512 : 1024;

  //this frame contains mixed resolutions: normalize every scanline to 512-width
  if(width256 && width512) {
    for(uint y : range(240)) {
      auto line = data + pitch * y;
      if(widths[y] == 256) {
        auto source = &line[256];
        auto target = &line[512];
        for(uint x : range(256)) {
          auto color = *--source;
          *--target = color;
          *--target = color;
        }
      }
    }
  }

  if(region->value() == "NTSC") {
    data += 2 * 512;
    if(overscan()) data += 16 * 512;
    uint height = 224 << state.interlace;
    screen->refresh(data, pitch * sizeof(uint32), width, height);
  }

  if(region->value() == "PAL") {
    if(!overscan()) data -= 14 * 512;
    uint height = 240 << state.interlace;
    screen->refresh(data, pitch * sizeof(uint32), width, height);
  }
}

}
