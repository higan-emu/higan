#include <ruby/ruby.hpp>
//using namespace ruby;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>

#include <nall/instance.hpp>

namespace ruby {
  extern Video video;
  extern Audio audio;
  extern Input input;
}

namespace Information {
  static const string Name    = "byuu";
  static const string Version = "0.7";
  static const string License = "GPLv3";
  static const string Website = "https://byuu.org";
}

#include "resource/resource.hpp"
#include "emulator/emulator.hpp"
#include "program/program.hpp"
#include "input/input.hpp"
#include "presentation/presentation.hpp"
#include "settings/settings.hpp"

auto locate(const string& name) -> string;
