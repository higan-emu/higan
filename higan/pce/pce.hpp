#pragma once

//license: GPLv3
//started: 2017-01-11

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/huc6280/huc6280.hpp>

namespace higan::PCEngine {
  extern Scheduler scheduler;
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

  struct Model {
    inline static auto PCEngine() -> bool;
    inline static auto SuperGrafx() -> bool;
  };

  #include <pce/controller/controller.hpp>

  #include <pce/cpu/cpu.hpp>
  #include <pce/vpc/vpc.hpp>
  #include <pce/vce/vce.hpp>
  #include <pce/vdc/vdc.hpp>
  #include <pce/psg/psg.hpp>

  #include <pce/system/system.hpp>
  #include <pce/cartridge/cartridge.hpp>
}

#include <pce/interface/interface.hpp>
