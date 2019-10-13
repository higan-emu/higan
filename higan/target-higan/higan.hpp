#include <ruby/ruby.hpp>
using namespace ruby;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern shared_pointer<higan::Interface> interface;
extern vector<shared_pointer<higan::Interface>> interfaces;

#include <nall/instance.hpp>

namespace nall::Path {
  extern string settings;   // ~/.local/share/higan/
  extern string templates;  // ~/.local/share/higan/Systems/
  extern string data;       // ~/higan/
}

extern Video videoInstance;
extern Audio audioInstance;
extern Input inputInstance;

#include "resource/resource.hpp"
#include "menus/menus.hpp"
#include "program/program.hpp"
#include "settings/settings.hpp"
#include "input/input.hpp"
#include "emulator/emulator.hpp"
