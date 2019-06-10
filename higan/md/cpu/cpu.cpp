#include <md/md.hpp>

namespace higan::MegaDrive {

CPU cpu;
#include "bus.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
  if(state.interruptPending) {
    if(state.interruptPending.bit((uint)Interrupt::Reset)) {
      state.interruptPending.bit((uint)Interrupt::Reset) = 0;
      r.a[7] = read(1, 1, 0) << 16 | read(1, 1, 2) << 0;
      r.pc   = read(1, 1, 4) << 16 | read(1, 1, 6) << 0;
      r.irc  = read(1, 1, r.pc & ~1);
      r.pc  += 2;
    }

    if(state.interruptPending.bit((uint)Interrupt::HorizontalBlank)) {
      if(4 > r.i) {
        state.interruptPending.bit((uint)Interrupt::HorizontalBlank) = 0;
        return interrupt(Vector::Level4, 4);
      }
    }

    if(state.interruptPending.bit((uint)Interrupt::VerticalBlank)) {
      if(6 > r.i) {
        state.interruptPending.bit((uint)Interrupt::VerticalBlank) = 0;
        return interrupt(Vector::Level6, 6);
      }
    }
  }

  instruction();
}

auto CPU::idle(uint clocks) -> void {
  Thread::step(clocks);
}

auto CPU::wait(uint clocks) -> void {
  while(Thread::wait) {
    Thread::step(2);
    synchronize(vdp);
  }

  Thread::step(clocks);
  synchronize(apu);
  synchronize(vdp);
  synchronize(psg);
  synchronize(ym2612);
  if(MegaCD()) synchronize(mcd);
  for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::raise(Interrupt interrupt) -> void {
  if(!state.interruptLine.bit((uint)interrupt)) {
    state.interruptLine.bit((uint)interrupt) = 1;
    state.interruptPending.bit((uint)interrupt) = 1;
  }
}

auto CPU::lower(Interrupt interrupt) -> void {
  state.interruptLine.bit((uint)interrupt) = 0;
  state.interruptPending.bit((uint)interrupt) = 0;
}

auto CPU::power(bool reset) -> void {
  M68K::power();
  Thread::create(system.frequency() / 7.0, [&] {
    while(true) scheduler.synchronize(), main();
  });

  ram.allocate(64_KiB >> 1);

  tmssEnable = false;
  if(system.tmss->value()) {
    tmss.allocate(2_KiB >> 1);
    if(auto fp = platform->open(system.node, "tmss.rom", File::Read, File::Required)) {
      for(uint address : range(tmss.size())) tmss.program(address, fp->readm(2));
      tmssEnable = true;
    }
  }

  if(!reset) memory::fill(ram.data(), sizeof(ram));

  io = {};
  io.version = tmssEnable;
  io.romEnable = !tmssEnable;
  io.vdpEnable[0] = !tmssEnable;
  io.vdpEnable[1] = !tmssEnable;

  state = {};
  state.interruptPending.bit((uint)Interrupt::Reset) = 1;
}

}
