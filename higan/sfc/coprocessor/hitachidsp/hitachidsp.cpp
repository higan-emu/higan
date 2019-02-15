HitachiDSP hitachidsp;
#include "memory.cpp"
#include "serialization.cpp"

auto HitachiDSP::step(uint clocks) -> void {
  HG51B::step(clocks);
  Thread::step(clocks);
  synchronize(cpu);
}

auto HitachiDSP::halt() -> void {
  HG51B::halt();
  if(io.irq == 0) r.i = 1, cpu.r.irq = 1;
}

auto HitachiDSP::unload() -> void {
  rom.reset();
  ram.reset();
  Thread::destroy();
}

auto HitachiDSP::power() -> void {
  HG51B::power();
  Thread::create(Frequency, [&] {
    while(true) scheduler.synchronize(), main();
  });
  cpu.coprocessors.append(this);
}
