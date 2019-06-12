#pragma once

//license: GPLv3
//started: 2017-01-11

#include <emulator/emulator.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/huc6280/huc6280.hpp>

namespace higan::PCEngine {
  #include <emulator/inline.hpp>
  extern Cheat cheat;

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
