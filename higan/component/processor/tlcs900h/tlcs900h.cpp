#include <emulator/emulator.hpp>
#include "tlcs900h.hpp"

namespace higan {

#define CF r.c
#define NF r.n
#define VF r.v
#define PF r.v
#define HF r.h
#define ZF r.z
#define SF r.s

#define RFP r.rfp
#define IFF r.iff

#include "registers.cpp"
#include "control-registers.cpp"
#include "memory.cpp"
#include "conditions.cpp"
#include "algorithms.cpp"
#include "dma.cpp"
#include "instruction.cpp"
#include "instructions.cpp"
#include "serialization.cpp"
#include "disassembler.cpp"

auto TLCS900H::interrupt(uint8 vector, uint3 priority) -> void {
  push(PC);
  push(SR);
  store(PC, load(Memory<uint24>{0xffff00 | vector}));
  store(INTNEST, load(INTNEST) + 1);
  r.iff = priority;
  if(r.iff != 7) r.iff++;
}

auto TLCS900H::power() -> void {
  r = {};
  r.xsp.l.l0 = 0x100;
}

}
