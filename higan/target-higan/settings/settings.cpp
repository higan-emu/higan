#include "../higan.hpp"
Settings settings;

auto Settings::load() -> void {
  if(auto document = file::read({Path::userSettings(), "higan/settings.bml"})) {
    unserialize(document);
  }
}

auto Settings::save() -> void {
  directory::create({Path::userSettings(), "higan/"});
  file::write({Path::userSettings(), "higan/settings.bml"}, serialize());
}

auto Settings::serialize() -> string {
  Markup::Node s;
  s("video/driver").setValue(video.driver);
  s("audio/driver").setValue(audio.driver);
  s("input/driver").setValue(input.driver);
  return BML::serialize(s);
}

auto Settings::unserialize(const string& markup) -> void {
  auto s = BML::unserialize(markup);
  s["video/driver"].value(video.driver);
  s["audio/driver"].value(audio.driver);
  s["input/driver"].value(input.driver);
}
