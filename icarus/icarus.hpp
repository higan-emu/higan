#include <nall/nall.hpp>
#include <nall/cd.hpp>
#include <nall/decode/cue.hpp>
#include <nall/decode/wav.hpp>
#include <nall/vfs.hpp>
using namespace nall;

#include <hiro/hiro.hpp>
using namespace hiro;

#include <higan/information.hpp>
#include <higan/resource/resource.hpp>

#include "resource/resource.hpp"

namespace icarus {
  #include "settings/settings.hpp"
  #include "media/media.hpp"
  #include "cartridge/cartridge.hpp"
  #include "compact-disc/compact-disc.hpp"
  #include "floppy-disk/floppy-disk.hpp"
  #include "program/program.hpp"

  extern vector<shared_pointer<Media>> media;
  auto construct() -> void;
  auto medium(const string& name) -> shared_pointer<Media>;
  auto identify(const string& filename) -> string;
}
