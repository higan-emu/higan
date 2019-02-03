#pragma once

//license: GPLv3
//started: 2004-10-14

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/random.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/arm7tdmi/arm7tdmi.hpp>
#include <component/processor/gsu/gsu.hpp>
#include <component/processor/hg51b/hg51b.hpp>
#include <component/processor/spc700/spc700.hpp>
#include <component/processor/upd96050/upd96050.hpp>
#include <component/processor/wdc65816/wdc65816.hpp>

#if defined(CORE_GB)
  #include <gb/gb.hpp>
#endif

namespace higan::SuperFamicom {
  extern Scheduler scheduler;
  extern Random random;
  extern Cheat cheat;

  struct Thread : higan::Thread {
    auto create(auto (*entrypoint)() -> void, double frequency) -> void {
      higan::Thread::create(entrypoint, frequency);
      scheduler.append(*this);
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }
  };

  struct Region {
    static inline auto NTSC() -> bool;
    static inline auto PAL() -> bool;
  };

  #include <sfc/system/system.hpp>
  #include <sfc/memory/memory.hpp>
  #include <sfc/ppu/counter/counter.hpp>

  #include <sfc/cpu/cpu.hpp>
  #include <sfc/smp/smp.hpp>
  #include <sfc/dsp/dsp.hpp>
  #include <sfc/ppu/ppu.hpp>
  #include <sfc/ppu-fast/ppu.hpp>

  #include <sfc/controller/controller.hpp>
  #include <sfc/expansion/expansion.hpp>
  #include <sfc/coprocessor/coprocessor.hpp>
  #include <sfc/slot/slot.hpp>
  #include <sfc/cartridge/cartridge.hpp>

  #include <sfc/memory/memory-inline.hpp>
  #include <sfc/ppu/counter/counter-inline.hpp>
}

#include <sfc/interface/interface.hpp>
