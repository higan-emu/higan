namespace higan {

Video video;
#include "sprite.cpp"
#include "screen.cpp"

auto Video::reset(Interface* interface) -> void {
  this->interface = interface;
  screens.reset();
}

auto Video::append(Screen& screen) -> void {
  if(screens.find(&screen)) return;
  screens.append(&screen);
}

auto Video::remove(Screen& screen) -> void {
  removeWhere(screens) == &screen;
}

}
