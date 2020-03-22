#include <nall/nall.hpp>
#include <nall/cd.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace hiro;

#include "resource/resource.hpp"

namespace icarus {
  static const string Name      = "icarus";
  static const string Version   = "110";
  static const string Copyright = "byuu et al";
  static const string License   = "GPLv3 or later";
  static const string Website   = "https://github.com/higan-emu";

  #include "settings/settings.hpp"
  #include "media/media.hpp"
  #include "cartridge/cartridge.hpp"
  #include "compact-disc/compact-disc.hpp"
  #include "floppy-disk/floppy-disk.hpp"
  #include "program/program.hpp"

  extern vector<shared_pointer<Media>> media;
  auto construct() -> void;
}
