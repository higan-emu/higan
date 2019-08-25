namespace higan {

Video video;
#include "sprite.cpp"
#include "screen.cpp"

auto Video::reset(Interface* interface) -> void {
  this->interface = interface;
  screens.reset();
}

auto Video::attached(shared_pointer<Screen>& screen) const -> bool {
  return (bool)screens.find(screen);
}

auto Video::attach(shared_pointer<Screen>& screen, Node::Video node) -> void {
  if(attached(screen)) return;
  screen = shared_pointer<Screen>{new Screen{node}};
  screens.append(screen);
}

auto Video::detach(shared_pointer<Screen>& screen) -> void {
  if(!attached(screen)) return;
  screens.removeByValue(screen);
  screen.reset();
}

auto Video::setPalette() -> void {
  for(auto& screen : screens) screen->setPalette();
}

auto Video::setFormat(uint32 red, uint32 green, uint32 blue) -> void {
  for(auto& screen : screens) screen->setFormat(red, green, blue);
}

auto Video::setSaturation(double saturation) -> void {
  for(auto& screen : screens) screen->setSaturation(saturation);
}

auto Video::setGamma(double gamma) -> void {
  for(auto& screen : screens) screen->setGamma(gamma);
}

auto Video::setLuminance(double luminance) -> void {
  for(auto& screen : screens) screen->setLuminance(luminance);
}

auto Video::setColorBleed(bool colorBleed) -> void {
  for(auto& screen : screens) screen->setColorBleed(colorBleed);
}

auto Video::setInterframeBlending(bool interframeBlending) -> void {
  for(auto& screen : screens) screen->setInterframeBlending(interframeBlending);
}

auto Video::setRotateLeft(bool rotateLeft) -> void {
  for(auto& screen : screens) screen->setRotateLeft(rotateLeft);
}

}
