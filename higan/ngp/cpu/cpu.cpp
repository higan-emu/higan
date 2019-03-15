#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

bool tracing=false;

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "pins.cpp"
#include "interrupts.cpp"
#include "timers.cpp"
#include "adc.cpp"
#include "rtc.cpp"
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

auto pc=r.pc.l.l0;
if(pc==0x2d8000){
static bool once=false;
if(!once){once=true;
//instructionSoftwareInterrupt(1);
//r.xwa[3].b.b1=0x02;
//r.xhl[3].l.l0=0x5000;
//r.xbc[3].b.b1=0x04;
//r.xbc[3].b.b0=0x00;
//tracing=true;
}}

static uint ctr=0;
if(tracing&&++ctr<300)print(disassemble(),"\n");

  if(r.halted) return step(16);

  instruction();
  step(1 << clock.rate);
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  timers.step(clocks);
  adc.step(clocks);
  rtc.step(clocks);
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

  r.pc.b.b0 = system.bios.read(0xff00);
  r.pc.b.b1 = system.bios.read(0xff01);
  r.pc.b.b2 = system.bios.read(0xff02);
  r.pc.b.b3 = 0x00;
  r.pc.l.l0 = 0xff1800;

  pin32.pin = 1;
  pin32.flow = 1;

  pin33.pin = 1;
  pin33.flow = 1;

  pin34.pin = 1;
  pin34.flow = 1;
  pin34.mode = 1;

  pin35.pin = 1;
  pin35.flow = 1;
  pin35.mode = 1;

  pin36.pin = 1;
  pin36.flow = 1;

  pin37.pin = 1;
  pin37.flow = 1;

  pin38.pin = 1;
  pin38.flow = 1;
  pin38.mode = 1;

  pin39.pin = 1;
  pin39.flow = 1;
  pin39.mode = 1;

  pin40.pin = 1;
  pin40.flow = 1;

  pin41.pin = 1;
  pin41.flow = 1;

  pin42.pin = 1;
  pin42.flow = 1;

  pin43.pin = 1;
  pin43.flow = 1;

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
  timers.timer01.ff.edge = [&](bool line) { if(to1.mode == 1) to1 = line; };
  timers.timer23.lo.interrupt = intt2;
  timers.timer23.hi.interrupt = intt3;
  timers.timer23.ff.edge = [&](bool line) { if(to3.mode == 1) to3 = line; };
  timers.timer4.interruptA = inttr4;
  timers.timer4.interruptB = inttr5;
  timers.timer4.outputA = [&](bool line) { if(to4.mode == 1) to4 = line; };
  timers.timer4.outputB = [&](bool line) { if(to5.mode == 1) to5 = line; };
  timers.timer5.interruptA = inttr6;
  timers.timer5.interruptB = inttr7;
  timers.timer5.outputA = [&](bool line) { if(to6.mode == 1) to6 = line; };
  timers.timer5.outputB = [&](bool line) {};  //pin does not exist

  cs0.enable  = 0;
  cs0.width   = 0;
  cs0.wait    = 0;
  cs0.address = 0xff0000;
  cs0.mask    = 0x1fffff;

  cs1.enable  = 0;
  cs1.width   = 0;
  cs1.wait    = 0;
  cs1.address = 0xff0000;
  cs1.mask    = 0x3fffff;

  cs2.enable  = 0;  //note: manual states this is 1; but that would block the BIOS at boot time ...
  cs2.width   = 0;
  cs2.wait    = 0;
  cs2.address = 0xff0000;
  cs2.mask    = 0x7fffff;
  cs2.mode    = 0;

  cs3.enable  = 0;
  cs3.width   = 0;
  cs3.wait    = 0;
  cs3.address = 0xff0000;
  cs3.mask    = 0x7fffff;
  cs3.cas     = 0;

  csx.width  = 0;
  csx.wait   = 0;

  clock.rate = 4;  //unconfirmed

  io = {};
}

auto CPU::load() -> void {
  ram.allocate(0x3000);
  if(auto fp = platform->open(system.node, "cpu.ram", File::Read)) {
    ram.load(fp);
  }
}

auto CPU::save() -> void {
  if(auto fp = platform->open(system.node, "cpu.ram", File::Write)) {
    ram.save(fp);
  }
}

auto CPU::unload() -> void {
  ram.reset();
}

}
