#include <pce/pce.hpp>

namespace higan::PCEngine {

CPU cpu;
#include "io.cpp"
#include "irq.cpp"
#include "timer.cpp"
#include "serialization.cpp"

auto CPU::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "CPU");
  from = Node::scan(parent = node, from);

  eventInstruction = Node::append<Node::Instruction>(parent, from, "Instruction", "CPU");
  eventInstruction->setAddressBits(21);

  eventInterrupt = Node::append<Node::Notification>(parent, from, "Interrupt", "CPU");

  if(Model::PCEngine())   ram.allocate( 8_KiB, 0x00);
  if(Model::SuperGrafx()) ram.allocate(32_KiB, 0x00);
  bram.allocate(2_KiB);
}

auto CPU::unload() -> void {
  ram.reset();
  bram.reset();

  node = {};
  eventInstruction = {};
  eventInterrupt = {};
}

auto CPU::main() -> void {
  if(irq.pending()) {
    if(eventInterrupt->enabled()) eventInterrupt->notify({"IRQ: $", hex(irq.vector(), 4L)});
    return interrupt(irq.vector());
  }

  if(eventInstruction->enabled()) {
    uint8 bank = r.mpr[r.pc.bit(13,15)];
    uint13 address = r.pc.bit(0,12);
    if(eventInstruction->address(bank << 13 | address)) {
      eventInstruction->notify(disassembleInstruction(), disassembleContext(), {
        "V:", pad(vdp.io.vcounter, 3L), " ", "H:", pad(vdp.io.hcounter, 4L)
      });
    }
  }
  instruction();
}

auto CPU::step(uint clocks) -> void {
  Thread::step(clocks);
  timer.step(clocks);
  synchronize(vdp);
  synchronize(psg);
//for(auto peripheral : peripherals) synchronize(*peripheral);
}

auto CPU::power() -> void {
  HuC6280::power();
  Thread::create(system.colorburst() * 2.0, {&CPU::main, this});

  r.pc.byte(0) = read(0x00, 0x1ffe);
  r.pc.byte(1) = read(0x00, 0x1fff);

  irq = {};
  timer = {};
  io = {};
}

auto CPU::lastCycle() -> void {
  irq.poll();
}

}
