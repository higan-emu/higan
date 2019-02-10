#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

/*auto Tree::CPU::initialize(Node::Object parent) -> void {
  node = Node::Component::create("CPU");
  version = Node::Natural::create("Version", 2);
  version->allowedValues = {1, 2};
  node->append(version);
  parent->append(node);
}*/

CPU cpu;
#include "dma.cpp"
#include "memory.cpp"
#include "io.cpp"
#include "timing.cpp"
#include "irq.cpp"
#include "serialization.cpp"

auto CPU::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Component::create("CPU"));
  from = Node::load(node, from);

  node->append(version = Node::Natural::create("Version", 2));
  version->allowedValues = {1, 2};
  Node::load(version, from);
}

auto CPU::main() -> void {
  if(r.wai) return instructionWait();
  if(r.stp) return instructionStop();

  if(status.interruptPending) {
    status.interruptPending = false;
    if(status.nmiPending) {
      status.nmiPending = false;
      r.vector = r.e ? 0xfffa : 0xffea;
      interrupt();
    } else if(status.irqPending) {
      status.irqPending = false;
      r.vector = r.e ? 0xfffe : 0xffee;
      interrupt();
    } else if(status.resetPending) {
      status.resetPending = false;
      step(132);
      r.vector = 0xfffc;
      interrupt();
    } else if(status.powerPending) {
      status.powerPending = false;
      step(186);
      r.pc.byte(0) = bus.read(0xfffc, r.mdr);
      r.pc.byte(1) = bus.read(0xfffd, r.mdr);
    }
  }

  instruction();
}

auto CPU::map() -> void {
  function<auto (uint24, uint8) -> uint8> reader;
  function<auto (uint24, uint8) -> void> writer;

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
  create(system.cpuFrequency(), [&] {
    while(true) scheduler.synchronize(), main();
  });
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
  status.lineClocks = lineclocks();
  status.dramRefreshPosition = (io.version == 1 ? 530 : 538);
  status.hdmaSetupPosition = (io.version == 1 ? 12 + 8 - dmaCounter() : 12 + dmaCounter());
  status.hdmaPosition = 1104;
  status.powerPending = reset == 0;
  status.resetPending = reset == 1;
  status.interruptPending = true;
}

}
