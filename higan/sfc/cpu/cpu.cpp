#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

CPU cpu;
#include "dma.cpp"
#include "memory.cpp"
#include "io.cpp"
#include "timing.cpp"
#include "irq.cpp"
#include "serialization.cpp"

auto CPU::load(Node::Object parent, Node::Object from) -> void {
  logger.attach(tracer);
  tracer->setSource("cpu");
  tracer->setAddressBits(24);

  logger.attach(onInterrupt);
  onInterrupt->setSource("cpu");
  onInterrupt->setName("interrupt");

  node = Node::append<Node::Component>(parent, from, "CPU");
  from = Node::scan(parent = node, from);

  version = Node::append<Node::Natural>(parent, from, "Version", 2);
  version->setAllowedValues({1, 2});
}

auto CPU::unload() -> void {
  logger.detach(tracer);
  logger.detach(onInterrupt);
}

auto CPU::main() -> void {
  if(r.wai) return instructionWait();
  if(r.stp) return instructionStop();

  if(!status.interruptPending) {
    if(tracer->enabled() && tracer->address(r.pc.d)) {
      tracer->instruction(disassembleInstruction(), disassembleContext(), {
        "V:", pad(vcounter(), 3L), " ", "H:", pad(hcounter(), 4L), " I:", (uint)field()
      });
    }
    return instruction();
  }

  if(status.nmiPending) {
    status.nmiPending = 0;
    r.vector = r.e ? 0xfffa : 0xffea;
    if(onInterrupt->enabled()) onInterrupt->event("NMI");
    return interrupt();
  }

  if(status.irqPending) {
    status.irqPending = 0;
    r.vector = r.e ? 0xfffe : 0xffee;
    if(onInterrupt->enabled()) onInterrupt->event("IRQ");
    return interrupt();
  }

  if(status.resetPending) {
    status.resetPending = 0;
    step(132);
    r.vector = 0xfffc;
    if(onInterrupt->enabled()) onInterrupt->event("reset");
    return interrupt();  //H=186
  }

  status.interruptPending = 0;
}

auto CPU::map() -> void {
  function<uint8 (uint24, uint8)> reader;
  function<void  (uint24, uint8)> writer;

  reader = {&CPU::readRAM, this};
  writer = {&CPU::writeRAM, this};
  bus.map(reader, writer, "00-3f,80-bf:0000-1fff", 0x2000);
  bus.map(reader, writer, "7e-7f:0000-ffff", 0x20000);

  reader = {&CPU::readAPU, this};
  writer = {&CPU::writeAPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2140-217f");

  reader = {&CPU::readCPU, this};
  writer = {&CPU::writeCPU, this};
  bus.map(reader, writer, "00-3f,80-bf:2180-2183,4016-4017,4200-421f");

  reader = {&CPU::readDMA, this};
  writer = {&CPU::writeDMA, this};
  bus.map(reader, writer, "00-3f,80-bf:4300-437f");
}

auto CPU::power(bool reset) -> void {
  WDC65816::power();
  create(system.cpuFrequency(), {&CPU::main, this});
  coprocessors.reset();
  PPUcounter::reset();
  PPUcounter::scanline = {&CPU::scanline, this};

  if(!reset) random.array(wram, sizeof(wram));

  for(uint n : range(8)) {
    channels[n] = {};
    if(n != 7) channels[n].next = channels[n + 1];
  }

  counter = {};

  io = {};
  io.version = version->value();

  alu = {};

  status = {};
  status.dramRefreshPosition = (io.version == 1 ? 530 : 538);
  status.hdmaSetupPosition = (io.version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
  status.hdmaPosition = 1104;
  status.resetPending = 1;
  status.interruptPending = 1;
}

}
