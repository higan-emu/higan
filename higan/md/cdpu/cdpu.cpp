#include <md/md.hpp>

namespace higan::MegaDrive {

CDPU cdpu;
#include "bus.cpp"
#include "io.cpp"
#include "external.cpp"
#include "serialization.cpp"

auto CDPU::load() -> void {
  bios.allocate(128_KiB);
  pram.allocate(512_KiB);
  wram.allocate(256_KiB);
  bram.allocate(  8_KiB);

  if(expansion.node) {
    if(auto fp = platform->open(expansion.node, "program.rom", File::Read, File::Required)) {
      bios.load(fp);
    }
  }
}

auto CDPU::unload() -> void {
  bios.reset();
  pram.reset();
  wram.reset();
  bram.reset();
}

auto CDPU::main() -> void {
  if(io.halt) return step(16);

  if(irq.pending) {
    if(irq.reset.pending) {
      irq.reset.pending = 0;
      irq.synchronize();
      r.a[7] = pram[0] << 24 | pram[1] << 16 | pram[2] << 8 | pram[3] << 0;
      r.pc   = pram[4] << 24 | pram[5] << 16 | pram[6] << 8 | pram[7] << 0;
      return;
    }

    if(irq.graphics.pending && 1 > r.i) {
      irq.graphics.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level1, 1);
    }

    if(irq.external.pending && 2 > r.i) {
      irq.external.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level2, 2);
    }

    if(irq.timer.pending && 3 > r.i) {
      irq.timer.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level3, 3);
    }

    if(irq.cdd.pending && 4 > r.i) {
      irq.cdd.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level4, 4);
    }

    if(irq.cdc.pending && 5 > r.i) {
      irq.cdc.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level5, 5);
    }

    if(irq.subcode.pending && 6 > r.i) {
      irq.subcode.pending = 0;
      irq.synchronize();
      return interrupt(Vector::Level6, 6);
    }
  }

  instruction();
}

auto CDPU::step(uint clocks) -> void {
  io.counter += clocks;
  while(io.counter >= 384) {
    io.counter -= 384;
    io.stopwatch++;
    if(io.timer && !--io.timer) {
      irq.timer.pending |= irq.timer.enable;
      irq.synchronize();
    }
  }

  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto CDPU::power(bool reset) -> void {
  M68K::bus = this;
  M68K::power();
  Thread::create(12'500'000, [&] {
    while(true) scheduler.synchronize(), main();
  });
  io = {};
  led = {};
  irq = {};
  irq.reset.pending = 1;
  irq.synchronize();
}

auto CDPU::IRQs::synchronize() -> void {
  pending = reset.pending | graphics.pending | external.pending | timer.pending | cdd.pending | cdc.pending | subcode.pending;
}

}
