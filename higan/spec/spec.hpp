#pragma once

#include <higan/higan.hpp>
#include <nall/decode/wav.hpp>
#include <component/processor/z80/z80.hpp>
#include <component/audio/ay38910/ay38910.hpp>

namespace higan::Spectrum {
  #include <higan/inline.hpp>

  struct Model {
    inline static auto Spectrum48k() -> bool;
    inline static auto Spectrum128() -> bool;
  };

  struct Region {
    inline static auto PAL() -> bool;
  };

  #include <spec/system/system.hpp>
  #include <spec/tape/tape.hpp>
  #include <spec/keyboard/keyboard.hpp>
  #include <spec/expansion/expansion.hpp>
  #include <spec/cpu/cpu.hpp>
  #include <spec/ula/ula.hpp>
  #include <spec/psg/psg.hpp>
}

#include <spec/interface/interface.hpp>
