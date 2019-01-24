#pragma once

//license: GPLv3
//started: 2019-01-03

#include <emulator/emulator.hpp>
#include <emulator/thread.hpp>
#include <emulator/scheduler.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/tlcs900h/tlcs900h.hpp>
#include <component/processor/z80/z80.hpp>
#include <component/audio/sn76489/sn76489.hpp>

namespace higan::NeoGeoPocket {
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
    inline static auto NeoGeoPocket() -> bool;
    inline static auto NeoGeoPocketColor() -> bool;
  };

  #include <ngp/system/system.hpp>
  #include <ngp/cartridge/cartridge.hpp>
  #include <ngp/cpu/cpu.hpp>
  #include <ngp/apu/apu.hpp>
  #include <ngp/vpu/vpu.hpp>
  #include <ngp/psg/psg.hpp>
}

#include <ngp/interface/interface.hpp>
