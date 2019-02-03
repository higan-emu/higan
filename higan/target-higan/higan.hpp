#include <ruby/ruby.hpp>
using namespace ruby;
extern Audio audio;
extern Input input;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern shared_pointer<higan::Interface> emulator;
extern vector<shared_pointer<higan::Interface>> emulators;

#include <nall/instance.hpp>

namespace Theme {
  static const Color BackgroundColor{ 56,  56,  56};
  static const Color ForegroundColor{255, 255, 255};
}

#include "resource/resource.hpp"
#include "configurations/configurations.hpp"
#include "system/system.hpp"
#include "viewport/viewport.hpp"
#include "program/program.hpp"

auto locate(string) -> string;
