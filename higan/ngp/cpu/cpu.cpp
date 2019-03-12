#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

bool tracing=false;

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "pin.cpp"
#include "interrupt.cpp"
#include "timers.cpp"
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
  nmi.test(i);

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
  timers.step(clocks);
  adc.step(clocks);
  Thread::step(clocks);
  synchronize(vpu);
  synchronize(apu);
  synchronize(psg);
}

auto CPU::pollPowerButton() -> void {
  platform->input(controls.power);
  nmi.set(!controls.power->value);
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
//r.pc.l.l0 = 0xff1800;

  nmi.maskable = 0;
  nmi.priority = 7;
  nmi.edge.rising = 0;
  nmi.edge.falling = 1;
  int0.enable = 0;
  int0.edge.rising = 1;
  int0.level.high = 0;
  int4.edge.rising = 1;
  int5.edge.rising = 1;
  int6.edge.rising = 1;
  int7.edge.rising = 1;
  intt0.edge.rising = 1;
  intt1.edge.rising = 1;
  intt2.edge.rising = 1;
  intt3.edge.rising = 1;
  intad.edge.rising = 1;
  inttr4.edge.rising = 1;
  inttr5.edge.rising = 1;
  inttr6.edge.rising = 1;
  inttr7.edge.rising = 1;

  timers = {};
  timers.timer01.lo.interrupt = intt0;
  timers.timer01.hi.interrupt = intt1;
  timers.timer01.ff.edge = [&](bool line) { setPin34(line); };
  timers.timer23.lo.interrupt = intt2;
  timers.timer23.hi.interrupt = intt3;
  timers.timer23.ff.edge = [&](bool line) { setPin35(line); };
  timers.timer4.interruptA = inttr4;
  timers.timer4.interruptB = inttr5;
  timers.timer4.outputA = [&](bool line) { setPin38(line); };
  timers.timer4.outputB = [&](bool line) { setPin39(line); };
  timers.timer5.interruptA = inttr6;
  timers.timer5.interruptB = inttr7;
  timers.timer5.outputA = [&](bool line) { setPin42(line); };
  timers.timer5.outputB = [&](bool line) {};  //pin does not exist

  io = {};
}

}
