#pragma once

//license: GPLv3
//started: 2016-07-08

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/random.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/m68k/m68k.hpp>
#include <component/processor/z80/z80.hpp>
#include <component/audio/sn76489/sn76489.hpp>

namespace higan::MegaDrive {
  extern Scheduler scheduler;
  extern Random random;
  extern Cheat cheat;

  struct Wait {
    enum : uint {
      VDP_DMA = 1 << 0,
    };
  };

  struct Thread : higan::Thread {
    auto create(double frequency, function<void ()> entryPoint) -> void {
      higan::Thread::create(frequency, entryPoint);
      scheduler.append(*this);
      wait = 0;
    }

    auto destroy() -> void {
      scheduler.remove(*this);
      higan::Thread::destroy();
    }

    inline auto synchronize(Thread& thread) -> void {
      if(clock() >= thread.clock()) scheduler.resume(thread);
    }

    uint wait = 0;
  };

  struct Region {
    inline static auto NTSCJ() -> bool;
    inline static auto NTSCU() -> bool;
    inline static auto PAL() -> bool;
  };

  #include <md/controller/controller.hpp>

  #include <md/cpu/cpu.hpp>
  #include <md/apu/apu.hpp>
  #include <md/vdp/vdp.hpp>
  #include <md/psg/psg.hpp>
  #include <md/ym2612/ym2612.hpp>

  #include <md/system/system.hpp>
  #include <md/cartridge/cartridge.hpp>
}

#include <md/interface/interface.hpp>
