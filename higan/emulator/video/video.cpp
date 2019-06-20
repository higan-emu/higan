namespace higan {

Video video;
#include "sprite.cpp"
#include "screen.cpp"

auto Video::reset(Interface* interface) -> void {
  this->interface = interface;
  screens.reset();
}

auto Video::createScreen(Node::Video node) -> shared_pointer<Screen> {
  auto screen = shared_pointer_make<Screen>(node);
  screens.append(screen);
  return screen;
}

auto Video::removeScreen(shared_pointer<Screen> screen) -> bool {
  if(auto index = screens.find(screen)) return screens.remove(*index), true;
  return false;
}

}
