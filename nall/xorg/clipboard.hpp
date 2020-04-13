#pragma once

#include <nall/xorg/xorg.hpp>

namespace nall::Clipboard {

inline auto clear() -> void {
  XDisplay display;
  XlibAtom atom = XInternAtom(display, "CLIPBOARD", XlibFalse);
  XSetSelectionOwner(display, atom, XlibNone, XlibCurrentTime);
}

}
