#pragma once

//license: GPLv3
//started: 2016-08-17

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/z80/z80.hpp>
#include <component/audio/sn76489/sn76489.hpp>

namespace higan::MasterSystem {
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

  struct Model {
    inline static auto ColecoVision() -> bool;
    inline static auto SG1000() -> bool;
    inline static auto SC3000() -> bool;
    inline static auto MasterSystem() -> bool;
    inline static auto GameGear() -> bool;
  };

  struct Region {
    inline static auto NTSC() -> bool;
    inline static auto PAL() -> bool;
  };

  #include <ms/controller/controller.hpp>

  #include <ms/cpu/cpu.hpp>
  #include <ms/vdp/vdp.hpp>
  #include <ms/psg/psg.hpp>

  #include <ms/system/system.hpp>
  #include <ms/cartridge/cartridge.hpp>
}

#include <ms/interface/interface.hpp>
