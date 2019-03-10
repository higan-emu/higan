#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

bool tracing=false;

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "interrupt.cpp"
#include "adc.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
  Interrupt empty;
  maybe<Interrupt&> i = empty;

  inttc3.test(i);
  inttc2.test(i);
  inttc1.test(i);
  inttc0.test(i);
  intad.test(i);
  inttx1.test(i);
  intrx1.test(i);
  inttx0.test(i);
  intrx0.test(i);
  inttr7.test(i);
  inttr6.test(i);
  inttr5.test(i);
  inttr4.test(i);
  intt3.test(i);
  intt2.test(i);
  intt1.test(i);
  intt0.test(i);
  int7.test(i);
  int6.test(i);
  int5.test(i);
  int4.test(i);
  int0.test(i);

  if(i->priority && i->priority >= r.iff) {
    if(tracing) print("* IRQ ", hex(i->vector, 2L), " ", i->priority, "\n");
    interrupt(i->vector, i->priority);
    i->fired();
    r.halted = false;
  }

  if(r.halted) return step(16);

static uint ctr=0;
if(tracing&&++ctr<5000) print(disassemble(), "\n");
else tracing=false;

  instruction();
  step(1);
}

auto CPU::step(uint clocks) -> void {
  adc.step(clocks);
  Thread::step(clocks);
  synchronize(vpu);
  synchronize(apu);
  synchronize(psg);
}

auto CPU::power() -> void {
  TLCS900H::power();
  Thread::create(system.frequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
  ram.allocate(0x3000);
  r.pc.b.b0 = read(0xffff00);
  r.pc.b.b1 = read(0xffff01);
  r.pc.b.b2 = read(0xffff02);
  r.pc.b.b3 = read(0xffff03);
  r.pc.l.l0 = 0xff1800;

  int4.edge.rising = 1;
  int5.edge.rising = 1;
  intad.edge.rising = 1;

  io = {};
}

}
