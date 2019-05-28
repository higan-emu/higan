#include <nall/nall.hpp>
#include <nall/cd.hpp>
#include <hiro/hiro.hpp>
using namespace nall;
using namespace hiro;

namespace icarus {
  static const string Name    = "icarus";
  static const string Version = "106.135";
  static const string Author  = "byuu";
  static const string License = "GPLv3";
  static const string Website = "https://byuu.org/";
}

auto operator+=(string& lhs, const string& rhs) -> string& {
  lhs.append(rhs);
  return lhs;
}

#include "resource/resource.hpp"
#include "settings/settings.hpp"
#include "media/media.hpp"
#include "cartridge/cartridge.hpp"
#include "compact-disc/compact-disc.hpp"
#include "floppy-disk/floppy-disk.hpp"
#include "program/program.hpp"
