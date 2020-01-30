#include "../byuu.hpp"
#include "platform.cpp"
#include "load.cpp"

Program program;

auto Program::create() -> void {
  higan::platform = this;

  ruby::video.create(ruby::Video::optimalDriver());
  ruby::video.setContext(presentation.viewport.handle());

  ruby::audio.create(ruby::Audio::optimalDriver());
  ruby::audio.setContext(presentation.viewport.handle());

  ruby::input.create(ruby::Input::optimalDriver());
  ruby::input.setContext(presentation.viewport.handle());
}

auto Program::main() -> void {
  if(Application::modal()) return;  //modal loop calls usleep() internally
  if(!emulator) return (void)usleep(20 * 1000);
  emulator->interface->run();
}

auto Program::quit() -> void {
  Application::quit();

  ruby::video.reset();
  ruby::audio.reset();
  ruby::input.reset();
}
