#include <md/md.hpp>

namespace higan::MegaDrive {

static uint16 Unmapped = 0;

MCD mcd;
#include "bus.cpp"
#include "bus-external.cpp"
#include "io.cpp"
#include "io-external.cpp"
#include "irq.cpp"
#include "cdc.cpp"
#include "cdc-transfer.cpp"
#include "cdd.cpp"
#include "cdd-dac.cpp"
#include "timer.cpp"
#include "gpu.cpp"
#include "pcm.cpp"
#include "serialization.cpp"

auto MCD::load(Node::Object parent, Node::Object from) -> void {
  logger.attach(tracer);
  tracer->setSource("mcd");
  tracer->setAddressBits(24);

  logger.attach(onInterrupt);
  onInterrupt->setSource("mcd");
  onInterrupt->setName("interrupt");

  tray = Node::append<Node::Port>(parent, from, "Disc Tray", "Mega CD");
  tray->hotSwappable = true;
  tray->allocate = [&] { return Node::Peripheral::create("Mega CD"); };
  tray->attach = [&](auto node) { connect(node); };
  tray->detach = [&](auto node) { disconnect(); };
  tray->scan(from);

  bios.allocate  (128_KiB >> 1);
  pram.allocate  (512_KiB >> 1);
  wram.allocate  (256_KiB >> 1);
  bram.allocate  (  8_KiB);
  cdc.ram.allocate(16_KiB >> 1);

  if(expansion.node) {
    if(auto fp = platform->open(expansion.node, "backup.ram", File::Read)) {
      bram.load(fp);
    }
  }

  cdd.load(parent, from);
  pcm.load(parent, from);
}

auto MCD::unload() -> void {
  if(expansion.node) {
    if(auto fp = platform->open(expansion.node, "backup.ram", File::Write)) {
      bram.save(fp);
    }
  }

  logger.detach(tracer);
  logger.detach(onInterrupt);
  cdd.unload();
  pcm.unload();

  bios.reset();
  pram.reset();
  wram.reset();
  bram.reset();
  cdc.ram.reset();
}

auto MCD::connect(Node::Peripheral with) -> void {
  disconnect();
  if(with) {
    disc = Node::append<Node::Peripheral>(tray, with, "Mega CD");
    fd = platform->open(disc, "cd.rom", File::Read, File::Required);
    cdd.insert();
  }
}

auto MCD::disconnect() -> void {
  if(!disc) return;
  cdd.eject();
  disc = {};
  fd = {};
}

auto MCD::main() -> void {
  if(io.halt) return wait(16);

  if(irq.pending) {
    if(1 > r.i && gpu.irq.lower())      return interrupt(Vector::Level1, 1);
    if(2 > r.i && external.irq.lower()) return interrupt(Vector::Level2, 2);
    if(3 > r.i && timer.irq.lower())    return interrupt(Vector::Level3, 3);
    if(4 > r.i && cdd.irq.lower())      return interrupt(Vector::Level4, 4);
    if(5 > r.i && cdc.irq.lower())      return interrupt(Vector::Level5, 5);
    if(6 > r.i && irq.subcode.lower())  return interrupt(Vector::Level6, 6);
    if(irq.reset.lower()) {
      r.a[7] = read(1, 1, 0) << 16 | read(1, 1, 2) << 0;
      r.pc   = read(1, 1, 4) << 16 | read(1, 1, 6) << 0;
      prefetch();
      prefetch();
      return;
    }
  }

  if(tracer->enabled() && tracer->address(r.pc - 4)) {
    tracer->instruction(disassembleInstruction(r.pc - 4), disassembleContext());
  }

  instruction();
}

auto MCD::step(uint clocks) -> void {
  gpu.step(clocks);
  counter.divider += clocks;
  while(counter.divider >= 384) {
    counter.divider -= 384;
    cdc.clock();
    cdd.clock();
    timer.clock();
    pcm.clock();
  }
  counter.dma += clocks;
  while(counter.dma >= 6) {
    counter.dma -= 6;
    cdc.transfer.dma();
  }
  counter.pcm += clocks;
  while(counter.pcm >= frequency() / 44100.0) {
    counter.pcm -= frequency() / 44100.0;
    cdd.sample();
  }

  Thread::step(clocks);
}

auto MCD::idle(uint clocks) -> void {
  step(clocks);
}

auto MCD::wait(uint clocks) -> void {
  step(clocks);
  Thread::synchronize(cpu);
}

auto MCD::power(bool reset) -> void {
  if(auto fp = platform->open(expansion.node, "program.rom", File::Read, File::Required)) {
    for(uint address : range(bios.size())) bios.program(address, fp->readm(2));
  }

  M68K::power();
  Thread::create(12'500'000, {&MCD::main, this});
  counter = {};
  if(!reset) {
    io = {};
    led = {};
    irq = {};
    cdc.power(reset);
    cdd.power(reset);
    timer.power(reset);
    gpu.power(reset);
    pcm.power(reset);
  }
  irq.reset.enable = 1;
  irq.reset.raise();
  bios.program(0x72 >> 1, 0xffff);
}

}
