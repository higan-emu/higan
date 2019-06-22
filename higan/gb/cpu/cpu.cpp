#include <gb/gb.hpp>

namespace higan::GameBoy {

#define SP r.sp.word
#define PC r.pc.word

#include "io.cpp"
#include "memory.cpp"
#include "timing.cpp"
#include "serialization.cpp"
CPU cpu;

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
}

}
