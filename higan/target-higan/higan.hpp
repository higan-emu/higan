#include <ruby/ruby.hpp>
using namespace ruby;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern shared_pointer<higan::Interface> interface;
extern vector<shared_pointer<higan::Interface>> interfaces;

#include <nall/instance.hpp>

namespace Theme {
  static const Color BackgroundColor;  //{ 56,  56,  56};
  static const Color ForegroundColor;  //{255, 255, 255};
}

namespace nall::Path {
  extern string settings;   // ~/.local/share/higan/
  extern string templates;  // ~/.local/share/higan/Systems/
  extern string data;       // ~/higan/
}

#include "resource/resource.hpp"
#include "configurations/configurations.hpp"
#include "system/system.hpp"
#include "input/input.hpp"
#include "viewport/viewport.hpp"
#include "emulator/emulator.hpp"
