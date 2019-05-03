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
#include "cdd.cpp"
#include "timer.cpp"
#include "gpu.cpp"
#include "pcm.cpp"
#include "serialization.cpp"

auto MCD::load(Node::Object parent, Node::Object from) -> void {
  tray = Node::Port::create("Disc Tray", "Mega CD");
  tray->hotSwappable = true;
  tray->allocate = [&] { return Node::Peripheral::create("Mega CD"); };
  tray->attach = [&](auto node) { connect(node); };
  tray->detach = [&](auto node) { disconnect(); };
  if(from = Node::load(tray, from)) {
    if(auto node = from->find<Node::Peripheral>(0)) tray->connect(node);
  }
  parent->append(tray);
}

auto MCD::connect(Node::Peripheral with) -> void {
  disconnect();
  if(with) {
    disc = Node::Peripheral::create("Mega CD");
    disc->load(with);
    tray->prepend(disc);
    cdd.io.status = CDD::Status::Stopped;
  }
}

auto MCD::disconnect() -> void {
  if(!disc) return;
  disc = {};
  cdd.io.status = CDD::Status::NoDisc;
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
    if(1 > r.i && gpu.irq.lower())      return interrupt(Vector::Level1, 1);
    if(2 > r.i && external.irq.lower()) return interrupt(Vector::Level2, 2);
    if(3 > r.i && timer.irq.lower())    return interrupt(Vector::Level3, 3);
    if(4 > r.i && cdd.irq.lower())      return interrupt(Vector::Level4, 4);
    if(5 > r.i && cdc.irq.lower())      return interrupt(Vector::Level5, 5);
    if(6 > r.i && irq.subcode.lower())  return interrupt(Vector::Level6, 6);
    if(irq.reset.lower()) {
      r.a[7] = read(1, 1, 0) << 16 | read(1, 1, 2) << 0;
      r.pc   = read(1, 1, 4) << 16 | read(1, 1, 6) << 0;
      return;
    }
  }

//static uint ctr=0;if(++ctr>2000000)print(disassembleRegisters(), "\n", disassemble(r.pc), "\n\n");

  instruction();
}

auto MCD::step(uint clocks) -> void {
  gpu.step(clocks);
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
  bios.allocate  (128_KiB >> 1);
  pram.allocate  (512_KiB >> 1);
  wram.allocate  (256_KiB >> 1);
  bram.allocate  (  8_KiB);
  pcm.ram.allocate(64_KiB);

  if(auto fp = platform->open(expansion.node, "program.rom", File::Read, File::Required)) {
    for(uint address : range(bios.size())) bios.program(address, fp->readm(2));
  }

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
    gpu.power(reset);
    pcm.power(reset);
  }
  irq.reset.enable = 1;
  irq.reset.raise();
  bios.program(0x72 >> 1, 0xffff);
}

}
