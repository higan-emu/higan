#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

//bool tracing=false;

CPU cpu;
#include "memory.cpp"
#include "io.cpp"
#include "serialization.cpp"

auto CPU::main() -> void {
//static uint ctr=0;
//if(tracing&&++ctr<4000) print(disassemble(), "\n");
//else tracing=false;
  instruction();
  step(1);
}

auto CPU::step(uint clocks) -> void {
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
  io = {};
}

auto CPU::setInterruptAPU(boolean line) -> void {
  if(io.irq.apu != line) {
    io.irq.apu = line;
    if(line) interrupt(0xffff30);
  }
}

auto CPU::setInterruptHblank(boolean line) -> void {
  if(io.irq.hblank != line) {
    io.irq.hblank = line;
  //if(line) interrupt(0xffff0c);
  }
}

auto CPU::setInterruptVblank(boolean line) -> void {
  if(io.irq.vblank != line) {
    io.irq.vblank = line;
    if(line) interrupt(0xffff2c);
  }
}

}
