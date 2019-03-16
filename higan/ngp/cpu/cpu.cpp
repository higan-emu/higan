#include <ngp/ngp.hpp>

//#define PORT2

namespace higan::NeoGeoPocket {

bool tracing = false;

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "ports.cpp"
#include "interrupts.cpp"
#include "timers.cpp"
#include "adc.cpp"
#include "rtc.cpp"
#include "watchdog.cpp"
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
  intwd.test(i);
  nmi.test(i);

  if(i->priority && i->priority >= r.iff) {
    if(tracing) print("* IRQ ", hex(i->vector, 2L), " ", i->priority, "\n");
    i->fire();
    r.halted = false;
  }

//platform->input(controls.debug);
//if(!tracing&&controls.debug->value)tracing=true;

//auto pc=r.pc.l.l0;
//if(pc==0x2d8000){
//static bool once=false;
//if(!once){once=true;
//instructionSoftwareInterrupt(1);
//r.xwa[3].b.b1=0x02;
//r.xhl[3].l.l0=0x5000;
//r.xbc[3].b.b1=0x04;
//r.xbc[3].b.b0=0x00;
//tracing=true;
//}}

static uint ctr=0;
if(tracing&&++ctr<3000)print(disassemble(),"\n");

  if(r.halted) return step(16);

  instruction();
  step(2 << clock.rate);
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  prescaler.step(clocks);
  adc.step(clocks);
  rtc.step(clocks);
  watchdog.step(clocks);
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

  p10.latch = 1; p10.flow = 1;
  p11.latch = 1; p11.flow = 1;
  p12.latch = 1; p12.flow = 1;
  p13.latch = 1; p13.flow = 1;
  p14.latch = 1; p14.flow = 1;
  p15.latch = 1; p15.flow = 1;
  p16.latch = 1; p16.flow = 1;
  p17.latch = 1; p17.flow = 1;

  p20.latch = 1; p20.mode = 1;
  p21.latch = 1; p21.mode = 1;
  p22.latch = 1; p22.mode = 1;
  p23.latch = 1; p23.mode = 1;
  p24.latch = 1; p24.mode = 1;
  p25.latch = 1; p25.mode = 1;
  p26.latch = 1; p26.mode = 1;
  p27.latch = 1; p27.mode = 1;

  p52.latch = 1; p52.flow = 1; p52.mode = 1;
  p53.latch = 1; p53.flow = 1; p53.mode = 1;
  p54.latch = 1; p54.flow = 1; p54.mode = 1;
  p55.latch = 1; p55.flow = 1; p55.mode = 1;

  p60.latch = 1; p60.mode = 1;
  p61.latch = 1; p61.mode = 1;
  p62.latch = 1; p62.mode = 1;
  p63.latch = 1; p63.mode = 1;
  p64.latch = 1; p64.mode = 1;
  p65.latch = 1; p65.mode = 1;

  p70.latch = 1; p70.mode = 1;
  p71.latch = 1; p71.mode = 1;
  p72.latch = 1; p72.mode = 1;
  p73.latch = 1; p73.mode = 1;
  p74.latch = 1; p74.mode = 1;
  p75.latch = 1; p75.mode = 1;
  p76.latch = 1; p76.mode = 1;
  p77.latch = 1; p77.mode = 1;

  p80.latch = 1; p80.flow = 1; p80.mode = 1;
  p81.latch = 1; p81.flow = 1;
  p82.latch = 1; p82.flow = 1; p82.mode = 1;
  p83.latch = 1; p83.flow = 1; p83.mode = 1;
  p84.latch = 1; p84.flow = 1;
  p85.latch = 1; p85.flow = 1; p85.mode = 1;

  p90.latch = 1;
  p91.latch = 1;
  p92.latch = 1;
  p93.latch = 1;

  pa0.latch = 0; pa0.flow = 1;
  pa1.latch = 1; pa1.flow = 1;
  pa2.latch = 1; pa2.flow = 1; pa2.mode = 1;
  pa3.latch = 1; pa3.flow = 1; pa3.mode = 1;

  pb0.latch = 1; pb0.flow = 1;
  pb1.latch = 1; pb1.flow = 1;
  pb2.latch = 1; pb2.flow = 1; pb2.mode = 1;
  pb3.latch = 1; pb3.flow = 1; pb3.mode = 1;
  pb4.latch = 1; pb4.flow = 1;
  pb5.latch = 1; pb5.flow = 1;
  pb6.latch = 1; pb6.flow = 1; pb6.mode = 1;
  pb7.latch = 1; pb7.flow = 1;

  nmi.enable = 0;
  nmi.maskable = 0;
  nmi.priority = 7;
  nmi.line = 1;
  nmi.request = 0;
  nmi.edge.rising = 0;
  nmi.edge.falling = 1;

  intwd.enable = 1;
  intwd.maskable = 0;
  intwd.priority = 7;
  intwd.line = 1;
  intwd.request = 0;
  intwd.edge.falling = 1;

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
  inttc0.edge.rising = 1;
  inttc1.edge.rising = 1;
  inttc2.edge.rising = 1;
  inttc3.edge.rising = 1;

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

  cs2.enable  = 1;
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
    //somehow, this stops the BIOS from performing setup on subsequent boots ...
    ram[0x2c14] = 0xdd;
    ram[0x2c15] = 0x00;
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
