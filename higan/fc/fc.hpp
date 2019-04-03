#pragma once

//license: GPLv3
//started: 2011-09-05

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/mos6502/mos6502.hpp>
#include <component/audio/ym2149/ym2149.hpp>
#include <component/audio/ym2413/ym2413.hpp>

namespace higan::Famicom {
  extern Scheduler scheduler;
  extern Cheat cheat;

  struct Thread : higan::Thread {
    auto create(double frequency, function<void ()> entryPoint) -> void {
      higan::Thread::create(frequency, entryPoint);
      scheduler.append(*this);
    }

    auto destroy() -> void {
      scheduler.remove(*this);
      higan::Thread::destroy();
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }
  };

  struct Region {
    static inline auto NTSCJ() -> bool;
    static inline auto NTSCU() -> bool;
    static inline auto PAL() -> bool;
  };

  #include <fc/controller/controller.hpp>
  #include <fc/system/system.hpp>
  #include <fc/memory/memory.hpp>
  #include <fc/cartridge/cartridge.hpp>
  #include <fc/cpu/cpu.hpp>
  #include <fc/apu/apu.hpp>
  #include <fc/ppu/ppu.hpp>
}

#include <fc/interface/interface.hpp>
