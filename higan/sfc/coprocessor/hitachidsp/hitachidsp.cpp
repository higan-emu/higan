HitachiDSP hitachidsp;
#include "memory.cpp"
#include "serialization.cpp"

auto HitachiDSP::step(uint clocks) -> void {
  HG51B::step(clocks);
  Thread::step(clocks);
  Thread::synchronize(cpu);
}

auto HitachiDSP::halt() -> void {
  HG51B::halt();
  if(io.irq == 0) cpu.irq(r.i = 1);
}

auto HitachiDSP::unload() -> void {
  rom.reset();
  ram.reset();
  cpu.coprocessors.removeByValue(this);
  Thread::destroy();
}

auto HitachiDSP::power() -> void {
  HG51B::power();
  Thread::create(Frequency, [&] { main(); });
  cpu.coprocessors.append(this);
}
