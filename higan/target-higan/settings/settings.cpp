#include "../higan.hpp"
Settings settings;

auto Settings::load() -> void {
  if(auto document = file::read({Path::userSettings(), "higan/settings.bml"})) {
    properties(0, BML::unserialize(document));
  }
}

auto Settings::save() -> void {
  Markup::Node document;
  properties(1, document);
  directory::create({Path::userSettings(), "higan/"});
  file::write({Path::userSettings(), "higan/settings.bml"}, BML::serialize(document));
}

auto Settings::properties(bool mode, Markup::Node document) -> void {
  #define s(name) \
    if(mode == 0) document[string{#name}.transform(".", "/")].value(name); \
    if(mode == 1) document(string{#name}.transform(".", "/")).setValue(name);
  s(video.driver)
  s(video.format)
  s(video.exclusive)
  s(video.blocking)
  s(video.flush)
  s(video.luminance)
  s(video.saturation)
  s(video.gamma)
  s(video.showFrameRate)
  s(audio.driver)
  s(audio.device)
  s(audio.frequency)
  s(audio.latency)
  s(audio.exclusive)
  s(audio.blocking)
  s(audio.dynamic)
  s(audio.skew)
  s(audio.volume)
  s(audio.balance)
  s(audio.mute)
  s(input.driver)
  s(input.unfocused)
  #undef s
}
