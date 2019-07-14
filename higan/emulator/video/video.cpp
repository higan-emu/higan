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
  screens.removeByValue(&screen);
}

auto Video::setPalette() -> void {
  for(auto& screen : screens) screen->setPalette();
}

auto Video::setDepth(uint depth) -> void {
  for(auto& screen : screens) screen->setDepth(depth);
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
