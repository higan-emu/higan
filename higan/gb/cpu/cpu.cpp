#include <gb/gb.hpp>

namespace higan::GameBoy {

#define SP r.sp.word
#define PC r.pc.word

#include "io.cpp"
#include "memory.cpp"
#include "timing.cpp"
#include "serialization.cpp"
CPU cpu;

auto CPU::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Component>(parent, from, "CPU");
  from = Node::scan(parent = node, from);

  if(Model::GameBoy()) {
    version = Node::append<Node::String>(parent, from, "Version", "DMG-CPU B");
    version->setAllowedValues({
      "DMG-CPU",
      "DMG-CPU A",
      "DMG-CPU B",
      "DMG-CPU C",
      "CPU MGB",
    });
  }

  if(Model::SuperGameBoy()) {
    version = Node::append<Node::String>(parent, from, "Version", "SGB-CPU-01");
    version->setAllowedValues({
      "SGB-CPU-01",
      "CPU SGB2"
    });
  }

  if(Model::GameBoyColor()) {
    version = Node::append<Node::String>(parent, from, "Version", "CPU CGB");
    version->setAllowedValues({
      "CPU CGB",
      "CPU CGB A",
      "CPU CGB B",
      "CPU CGB C",
      "CPU CGB D",
      "CPU CGB E",
    });
  }
}

auto CPU::main() -> void {
  //are interrupts enabled?
  if(r.ime) {
    //are any interrupts pending?
    if(status.interruptLatch) {
      idle();
      idle();
      idle();
      r.ime = 0;
      write(--SP, PC >> 8);  //upper byte may write to IE before it is polled again
      uint8 mask = status.interruptFlag & status.interruptEnable;
      write(--SP, PC >> 0);  //lower byte write to IE has no effect
      if(mask) {
        uint interruptID = bit::first(mask);  //find highest priority interrupt
        lower(interruptID);
        PC = 0x0040 + interruptID * 8;
      } else {
        //if push(PCH) writes to IE and disables all requested interrupts, PC is forced to zero
        PC = 0x0000;
      }
    }
  }

  instruction();
}

auto CPU::raised(uint interruptID) const -> bool {
  return status.interruptFlag.bit(interruptID);
}

auto CPU::raise(uint interruptID) -> void {
  status.interruptFlag.bit(interruptID) = 1;
  if(status.interruptEnable.bit(interruptID)) {
    r.halt = false;
    if(interruptID == Interrupt::Joypad) r.stop = false;
  }
}

auto CPU::lower(uint interruptID) -> void {
  status.interruptFlag.bit(interruptID) = 0;
}

auto CPU::stop() -> bool {
  if(status.speedSwitch) {
    status.speedSwitch = 0;
    status.speedDouble ^= 1;
    if(status.speedDouble == 0) setFrequency(4 * 1024 * 1024);
    if(status.speedDouble == 1) setFrequency(8 * 1024 * 1024);
    return true;
  }
  return false;
}

auto CPU::power() -> void {
  Thread::create(4 * 1024 * 1024, {&CPU::main, this});
  SM83::power();

  for(uint n = 0xc000; n <= 0xdfff; n++) bus.mmio[n] = this;  //WRAM
  for(uint n = 0xe000; n <= 0xfdff; n++) bus.mmio[n] = this;  //WRAM (mirror)
  for(uint n = 0xff80; n <= 0xfffe; n++) bus.mmio[n] = this;  //HRAM

  bus.mmio[0xff00] = this;  //JOYP
  bus.mmio[0xff01] = this;  //SB
  bus.mmio[0xff02] = this;  //SC
  bus.mmio[0xff04] = this;  //DIV
  bus.mmio[0xff05] = this;  //TIMA
  bus.mmio[0xff06] = this;  //TMA
  bus.mmio[0xff07] = this;  //TAC
  bus.mmio[0xff0f] = this;  //IF
  bus.mmio[0xffff] = this;  //IE

  if(Model::GameBoyColor()) {
  bus.mmio[0xff4d] = this;  //KEY1
  bus.mmio[0xff51] = this;  //HDMA1
  bus.mmio[0xff52] = this;  //HDMA2
  bus.mmio[0xff53] = this;  //HDMA3
  bus.mmio[0xff54] = this;  //HDMA4
  bus.mmio[0xff55] = this;  //HDMA5
  bus.mmio[0xff56] = this;  //RP
  bus.mmio[0xff6c] = this;  //???
  bus.mmio[0xff70] = this;  //SVBK
  bus.mmio[0xff72] = this;  //???
  bus.mmio[0xff73] = this;  //???
  bus.mmio[0xff74] = this;  //???
  bus.mmio[0xff75] = this;  //???
  bus.mmio[0xff76] = this;  //???
  bus.mmio[0xff77] = this;  //???
  }

  for(auto& n : wram) n = 0x00;
  for(auto& n : hram) n = 0x00;

  status = {};

  //0146~0149
  if(version->latch() == "DMG-CPU"   ) status.div = 0x0146;

  //ffe6~ffe9
  if(version->latch() == "DMG-CPU A" ) status.div = 0xffe6;
  if(version->latch() == "DMG-CPU B" ) status.div = 0xffe6;
  if(version->latch() == "DMG-CPU C" ) status.div = 0xffe6;
  if(version->latch() == "CPU MGB"   ) status.div = 0xffe6;

  if(version->latch() == "SGB-CPU-01") status.div = 0xffe6;  //unconfirmed
  if(version->latch() == "CPU SGB2"  ) status.div = 0xffe6;  //unconfirmed

  //0206~0209
  if(version->latch() == "CPU CGB"   ) status.div = 0x0206;

  //fffa~fffd
  if(version->latch() == "CPU CGB A" ) status.div = 0xfffa;
  if(version->latch() == "CPU CGB B" ) status.div = 0xfffa;
  if(version->latch() == "CPU CGB C" ) status.div = 0xfffa;
  if(version->latch() == "CPU CGB D" ) status.div = 0xfffa;
  if(version->latch() == "CPU CGB E" ) status.div = 0xfffa;
}

}
