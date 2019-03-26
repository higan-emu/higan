#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

APU apu;
#include "memory.cpp"
#include "serialization.cpp"

auto APU::main() -> void {
  if(!io.enable) return step(16);

  if(nmi.line) {
    nmi.line = 0;  //edge-sensitive
    Z80::irq(0, 0x0066, 0xff);
  }

  if(irq.line) {
    //level-sensitive
    Z80::irq(1, 0x0038, 0xff);
  }

  instruction();
}

auto APU::step(uint clocks) -> void {
  Thread::step(clocks);
  synchronize(cpu);
  synchronize(psg);
}

auto APU::synchronizing() const -> bool {
  return scheduler.synchronizing();
}

auto APU::power() -> void {
  Z80::bus = this;
  Z80::power();
  bus->grant(false);
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });

  nmi = {};
  irq = {};
  io = {};
  io.enable = false;
}

auto APU::enable() -> void {
  Thread::destroy();
  Z80::power();
  bus->grant(false);
  Thread::create(system.frequency() / 2.0, [&] {
    while(true) scheduler.synchronize(), main();
  });

  nmi = {};
  irq = {};
  io = {};
  io.enable = true;
}

auto APU::disable() -> void {
  io.enable = false;
}

auto APU::load() -> void {
  ram.allocate(4_KiB, 0x00);
  if(auto fp = platform->open(system.node, "apu.ram", File::Read)) {
    ram.load(fp);
  }
}

auto APU::save() -> void {
  if(auto fp = platform->open(system.node, "apu.ram", File::Write)) {
    ram.save(fp);
  }
}

auto APU::unload() -> void {
  ram.reset();
}

}
