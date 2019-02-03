#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

PPU ppu;
#include "io.cpp"
#include "background.cpp"
#include "object.cpp"
#include "window.cpp"
#include "screen.cpp"
#include "serialization.cpp"
#include "counter/serialization.cpp"

auto PPU::initialize(Node::Node parent) -> void {
  display = Node::Port::Video::create("Display");
  display->type   = "CRT";
  display->width  = 512;
  display->height = 480;
  display->aspect = 8.0 / 7.0;
  display->append(settings.colorEmulation = Node::Setting::Boolean::create("Color Emulation", true, [&](auto) {
    video.setPalette();
  }));
  display->append(settings.colorBleed = Node::Setting::Boolean::create("Color Bleed", true, [&](auto value) {
    video.setEffect(Video::Effect::ColorBleed, value);
  }));
  parent->append(display);
}

PPU::PPU() :
bg1(Background::ID::BG1),
bg2(Background::ID::BG2),
bg3(Background::ID::BG3),
bg4(Background::ID::BG4) {
  output = new uint32[512 * 512];
  output += 16 * 512;  //overscan offset
}

PPU::~PPU() {
  if(system.fastPPU()) {
    setHandle(nullptr);
  }

  output -= 16 * 512;
  delete[] output;
}

auto PPU::step(uint clocks) -> void {
  clocks >>= 1;
  while(clocks--) {
    tick(2);
    Thread::step(2);
    synchronize(cpu);
  }
}

auto PPU::Enter() -> void {
  while(true) scheduler.synchronize(), ppu.main();
}

auto PPU::main() -> void {
  scanline();
  step(28);
  bg1.begin();
  bg2.begin();
  bg3.begin();
  bg4.begin();

  if(vcounter() <= 239) {
    for(int pixel = -7; pixel <= 255; pixel++) {
      bg1.run(1);
      bg2.run(1);
      bg3.run(1);
      bg4.run(1);
      step(2);

      bg1.run(0);
      bg2.run(0);
      bg3.run(0);
      bg4.run(0);
      if(pixel >= 0) {
        obj.run();
        window.run();
        screen.run();
      }
      step(2);
    }

    step(14);
    obj.tilefetch();
  }

  step(lineclocks() - hcounter());
}

auto PPU::load() -> bool {
  if(system.fastPPU()) {
    return ppufast.load();
  }

  ppu1.version = property.ppu1.version();
  ppu2.version = property.ppu2.version();
  vram.mask = property.ppu1.vram.size() / sizeof(uint16) - 1;
  if(vram.mask != 0xffff) vram.mask = 0x7fff;
  return true;
}

auto PPU::power(bool reset) -> void {
  if(system.fastPPU()) {
    ppufast.power(reset);
    return setHandle(ppufast.handle());
  }

  create(Enter, system.cpuFrequency());
  PPUcounter::reset();
  memory::fill<uint32>(output, 512 * 480);

  function<auto (uint24, uint8) -> uint8> reader{&PPU::readIO, this};
  function<auto (uint24, uint8) -> void> writer{&PPU::writeIO, this};
  bus.map(reader, writer, "00-3f,80-bf:2100-213f");

  if(!reset) random.array((uint8*)vram.data, sizeof(vram.data));

  ppu1.mdr = random.bias(0xff);
  ppu2.mdr = random.bias(0xff);

  latch.vram = random();
  latch.oam = random();
  latch.cgram = random();
  latch.bgofsPPU1 = random();
  latch.bgofsPPU2 = random();
  latch.mode7 = random();
  latch.counters = false;
  latch.hcounter = 0;
  latch.vcounter = 0;

  latch.oamAddress = 0x0000;
  latch.cgramAddress = 0x00;

  //$2100  INIDISP
  io.displayDisable = true;
  io.displayBrightness = 0;

  //$2102  OAMADDL
  //$2103  OAMADDH
  io.oamBaseAddress = random();
  io.oamAddress = random();
  io.oamPriority = random();

  //$2105  BGMODE
  io.bgPriority = false;
  io.bgMode = 0;

  //$210d  BG1HOFS
  io.hoffsetMode7 = random();

  //$210e  BG1VOFS
  io.voffsetMode7 = random();

  //$2115  VMAIN
  io.vramIncrementMode = random.bias(1);
  io.vramMapping = random();
  io.vramIncrementSize = 1;

  //$2116  VMADDL
  //$2117  VMADDH
  io.vramAddress = random();

  //$211a  M7SEL
  io.repeatMode7 = random();
  io.vflipMode7 = random();
  io.hflipMode7 = random();

  //$211b  M7A
  io.m7a = random();

  //$211c  M7B
  io.m7b = random();

  //$211d  M7C
  io.m7c = random();

  //$211e  M7D
  io.m7d = random();

  //$211f  M7X
  io.m7x = random();

  //$2120  M7Y
  io.m7y = random();

  //$2121  CGADD
  io.cgramAddress = random();
  io.cgramAddressLatch = random();

  //$2133  SETINI
  io.extbg = random();
  io.pseudoHires = random();
  io.overscan = false;
  io.interlace = false;

  //$213c  OPHCT
  io.hcounter = 0;

  //$213d  OPVCT
  io.vcounter = 0;

  bg1.power();
  bg2.power();
  bg3.power();
  bg4.power();
  obj.power();
  window.power();
  screen.power();

  updateVideoMode();
  frame();
}

auto PPU::scanline() -> void {
  if(vcounter() == 0) {
    frame();
    bg1.frame();
    bg2.frame();
    bg3.frame();
    bg4.frame();
  }

  bg1.scanline();
  bg2.scanline();
  bg3.scanline();
  bg4.scanline();
  obj.scanline();
  window.scanline();
  screen.scanline();

  if(vcounter() == 240) {
    scheduler.exit(Scheduler::Event::Frame);
  }
}

auto PPU::frame() -> void {
  obj.frame();
  self.interlace = io.interlace;
  self.overscan = io.overscan;
}

auto PPU::refresh() -> void {
  if(system.fastPPU()) {
    return ppufast.refresh();
  }

  auto output = this->output;
  if(!overscan()) output -= 14 * 512;
  auto pitch = 512;
  auto width = 512;
  auto height = 480;
  video.setEffect(Video::Effect::ColorBleed, option.video.colorBleed());
  video.refresh(display, output, pitch * sizeof(uint32), width, height);
}

}
