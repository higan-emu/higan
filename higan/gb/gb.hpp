#pragma once

//license: GPLv3
//started: 2010-12-27

#include <emulator/emulator.hpp>
#include <emulator/cheat.hpp>

#include <component/processor/sm83/sm83.hpp>

namespace higan::GameBoy {
  #include <emulator/inline.hpp>
  extern Cheat cheat;

  struct Model {
    inline static auto GameBoy() -> bool;
    inline static auto GameBoyColor() -> bool;
    inline static auto SuperGameBoy() -> bool;
  };

  #include <gb/memory/memory.hpp>
  #include <gb/system/system.hpp>
  #include <gb/cartridge/cartridge.hpp>
  #include <gb/cpu/cpu.hpp>
  #include <gb/ppu/ppu.hpp>
  #include <gb/apu/apu.hpp>
}

#include <gb/interface/interface.hpp>
