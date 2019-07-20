#if 1
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

  step(28);
  bg1.begin();
  bg2.begin();
  bg3.begin();
  bg4.begin();

  for(int pixel = -7; pixel <= 255; pixel++) {
    bg1.run(1);
    bg2.run(1);
    bg3.run(1);
    bg4.run(1);
    step();

    bg1.run(0);
    bg2.run(0);
    bg3.run(0);
    bg4.run(0);
    if(pixel >= 0) {
      obj.run();
      window.run();
      screen.run();
    }
    step();
  }

  step(14 + 34 * 2);
  obj.tilefetch();
  step(lineclocks() - hcounter());
}
#endif

//note: comment out ppu.step() from object.cpp:obj.tilefetch() when using the below version.

#if 0
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

  #define cycles(index) \
    cycle<index +  0>(); cycle<index +  2>(); cycle<index +  4>(); cycle<index +  6>(); \
    cycle<index +  8>(); cycle<index + 10>(); cycle<index + 12>(); cycle<index + 14>(); \
    cycle<index + 16>(); cycle<index + 18>(); cycle<index + 20>(); cycle<index + 22>(); \
    cycle<index + 24>(); cycle<index + 26>(); cycle<index + 28>(); cycle<index + 30>(); \
    cycle<index + 32>(); cycle<index + 34>(); cycle<index + 36>(); cycle<index + 38>(); \
    cycle<index + 40>(); cycle<index + 42>(); cycle<index + 44>(); cycle<index + 46>(); \
    cycle<index + 48>(); cycle<index + 50>(); cycle<index + 52>(); cycle<index + 54>(); \
    cycle<index + 56>(); cycle<index + 58>(); cycle<index + 60>(); cycle<index + 62>()
  cycles(   0);
  cycles(  64);
  cycles( 128);
  cycles( 192);
  cycles( 256);
  cycles( 320);
  cycles( 384);
  cycles( 448);
  cycles( 512);
  cycles( 576);
  cycles( 640);
  cycles( 704);
  cycles( 768);
  cycles( 832);
  cycles( 896);
  cycles( 960);
  cycles(1024);
  cycles(1088);
  cycles(1152);
  cycles(1216);  //H = 1280
  step(lineclocks() - hcounter());
  #undef cycles
}

template<uint Cycle>
auto PPU::cycle() -> void {
  if constexpr(Cycle == 28) {
    bg1.begin();
    bg2.begin();
    bg3.begin();
    bg4.begin();
  }

  if constexpr(Cycle >= 28 && Cycle <= 1078 && (Cycle - 28) % 4 == 0) {
    bg1.run(1);
    bg2.run(1);
    bg3.run(1);
    bg4.run(1);
  }

  if constexpr(Cycle >= 28 && Cycle <= 1078 && (Cycle - 28) % 4 == 2) {
    bg1.run(0);
    bg2.run(0);
    bg3.run(0);
    bg4.run(0);
  }

  if constexpr(Cycle >= 56 && Cycle <= 1078 && (Cycle - 28) % 4 == 2) {
    obj.run();
    window.run();
    screen.run();
  }

  if constexpr(Cycle == 1160) {
    obj.tilefetch();
  }

  step();
}
#endif
