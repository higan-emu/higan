#include <md/md.hpp>

namespace higan::MegaDrive {

CPU cpu;
#include "bus.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
  if(state.interruptPending) {
    if(state.interruptPending.bit((uint)Interrupt::Reset)) {
      state.interruptPending.bit((uint)Interrupt::Reset) = 0;
      r.a[7] = bus->readWord(0) << 16 | bus->readWord(2) << 0;
      r.pc   = bus->readWord(4) << 16 | bus->readWord(6) << 0;
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

//static uint ctr=0;if(++ctr>5000000)print(disassembleRegisters(), "\n", disassemble(r.pc), "\n\n");

  instruction();
}

auto CPU::step(uint clocks) -> void {
  while(wait) {
    Thread::step(1);
    synchronize();
  }

  Thread::step(clocks);
  synchronize();
}

auto CPU::synchronize() -> void {
  synchronize(apu);
  synchronize(vdp);
  synchronize(psg);
  synchronize(ym2612);
  if(MegaCD()) {
    synchronize(cdpu);
  }
  for(auto peripheral : peripherals) {
    synchronize(*peripheral);
  }
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
  M68K::bus = this;
  M68K::power();
  Thread::create(system.frequency() / 7.0, [&] {
    while(true) scheduler.synchronize(), main();
  });

  ram.allocate(64_KiB);

  tmssEnable = false;
  if(system.tmss->value()) {
    tmss.allocate(2_KiB);
    if(auto fp = platform->open(system.node, "tmss.rom", File::Read, File::Required)) {
      tmss.load(fp);
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
