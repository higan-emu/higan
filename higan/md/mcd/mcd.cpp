#include <md/md.hpp>

namespace higan::MegaDrive {

MCD mcd;
#include "bus.cpp"
#include "io.cpp"
#include "external.cpp"
#include "irq.cpp"
#include "cdc.cpp"
#include "cdd.cpp"
#include "timer.cpp"
#include "pcm.cpp"
#include "serialization.cpp"

auto MCD::load() -> void {
  bios.allocate  (128_KiB);
  pram.allocate  (512_KiB);
  wram.allocate  (256_KiB);
  bram.allocate  (  8_KiB);
  pcm.ram.allocate(64_KiB);

  if(expansion.node) {
    if(auto fp = platform->open(expansion.node, "program.rom", File::Read, File::Required)) {
      bios.load(fp);
    }
  }
}

auto MCD::unload() -> void {
  bios.reset();
  pram.reset();
  wram.reset();
  bram.reset();
  pcm.ram.reset();
}

auto MCD::main() -> void {
  if(io.halt) return step(16);

  if(irq.pending) {
    if(1 > r.i && irq.graphics.acknowledge()) return interrupt(Vector::Level1, 1);
    if(2 > r.i && irq.external.acknowledge()) return interrupt(Vector::Level2, 2);
    if(3 > r.i && timer.irq.acknowledge())    return interrupt(Vector::Level3, 3);
    if(4 > r.i && cdd.irq.acknowledge())      return interrupt(Vector::Level4, 4);
    if(5 > r.i && cdc.irq.acknowledge())      return interrupt(Vector::Level5, 5);
    if(6 > r.i && irq.subcode.acknowledge())  return interrupt(Vector::Level6, 6);
    if(irq.reset.acknowledge()) {
      r.a[7] = pram[0] << 24 | pram[1] << 16 | pram[2] << 8 | pram[3] << 0;
      r.pc   = pram[4] << 24 | pram[5] << 16 | pram[6] << 8 | pram[7] << 0;
      return;
    }
  }

  instruction();
}

auto MCD::step(uint clocks) -> void {
  io.counter += clocks;
  while(io.counter >= 384) {
    io.counter -= 384;
    cdc.clock();
    cdd.clock();
    timer.clock();
    pcm.clock();
  }

  Thread::step(clocks);
  synchronize(cpu);
  synchronize(apu);
}

auto MCD::power(bool reset) -> void {
  M68K::power();
  Thread::create(12'500'000, [&] {
    while(true) scheduler.synchronize(), main();
  });
  if(!reset) {
    io = {};
    led = {};
    irq = {};
    cdc.power(reset);
    cdd.power(reset);
    timer.power(reset);
    pcm.power(reset);
  }
  irq.reset.enable = 1;
  irq.reset.raise();
}

}
