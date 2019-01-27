#include <ruby/ruby.hpp>
using namespace ruby;
extern Video video;
extern Audio audio;
extern Input input;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <emulator/emulator.hpp>
extern shared_pointer<higan::Interface> emulator;
extern vector<shared_pointer<higan::Interface>> emulators;

#include <nall/arguments.hpp>
#include <nall/instance.hpp>
#include <nall/decode/zip.hpp>

#include "settings/settings.hpp"
#include "program/program.hpp"
#include "presentation/presentation.hpp"
#include "resource/resource.hpp"

auto locate(string name) -> string;
