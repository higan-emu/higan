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

#include "resource/resource.hpp"
#include "emulator/emulator.hpp"
#include "program/program.hpp"
#include "presentation/presentation.hpp"
#include "settings/settings.hpp"

auto locate(const string& name) -> string;
