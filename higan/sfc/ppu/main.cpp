auto PPU::main() -> void {
  if(vcounter() == 0) {
    self.interlace = io.interlace;
    self.overscan = io.overscan;
    bg1.frame();
    bg2.frame();
    bg3.frame();
    bg4.frame();
    obj.frame();
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

  if(vcounter() > 240) {
    step(lineclocks());
    return;
  }

  #define cycles02(index) cycle<index>()
  #define cycles04(index) cycles02(index); cycles02(index +  2)
  #define cycles08(index) cycles04(index); cycles04(index +  4)
  #define cycles16(index) cycles08(index); cycles08(index +  8)
  #define cycles32(index) cycles16(index); cycles16(index + 16)
  #define cycles64(index) cycles32(index); cycles32(index + 32)
  cycles64(   0);
  cycles64(  64);
  cycles64( 128);
  cycles64( 192);
  cycles64( 256);
  cycles64( 320);
  cycles64( 384);
  cycles64( 448);
  cycles64( 512);
  cycles64( 576);
  cycles64( 640);
  cycles64( 704);
  cycles64( 768);
  cycles64( 832);
  cycles64( 896);
  cycles64( 960);
  cycles32(1024);
  cycles16(1056);
  cycles08(1072);
  obj.fetch();
  step(lineclocks() - hcounter());
}

//it would be lovely if we could put these functions inside cycle(),
//but due to the multiple template instantiations, that destroys L1 cache.
//it's a performance penalty of about 25% for the entire(!!) emulator.

auto PPU::cycleObjectEvaluate() -> void {
  obj.evaluate(hcounter() >> 3);
}

auto PPU::cycleBackgroundBegin() -> void {
  bg1.begin();
  bg2.begin();
  bg3.begin();
  bg4.begin();
}

auto PPU::cycleBackgroundBelow() -> void {
  bg1.run(1);
  bg2.run(1);
  bg3.run(1);
  bg4.run(1);
}

auto PPU::cycleBackgroundAbove() -> void {
  bg1.run(0);
  bg2.run(0);
  bg3.run(0);
  bg4.run(0);
}

auto PPU::cycleRenderPixel() -> void {
  obj.run();
  window.run();
  screen.run();
}

template<uint Cycle>
auto PPU::cycle() -> void {
  if constexpr(Cycle >=    0 && Cycle <= 1022 && (Cycle -    0) % 8 == 0) cycleObjectEvaluate();
  if constexpr(Cycle ==   28                                            ) cycleBackgroundBegin();
  if constexpr(Cycle >=   28 && Cycle <= 1078 && (Cycle -   28) % 4 == 0) cycleBackgroundBelow();
  if constexpr(Cycle >=   28 && Cycle <= 1078 && (Cycle -   28) % 4 == 2) cycleBackgroundAbove();
  if constexpr(Cycle >=   56 && Cycle <= 1078 && (Cycle -   28) % 4 == 2) cycleRenderPixel();
  step();
}
