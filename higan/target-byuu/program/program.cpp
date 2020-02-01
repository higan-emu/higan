#include "../byuu.hpp"
#include "platform.cpp"
#include "load.cpp"

Program program;

auto Program::create() -> void {
  higan::platform = this;

  ruby::video.create(settings.video.driver);
  ruby::video.setContext(presentation.viewport.handle());
  ruby::video.setBlocking(false);

  ruby::audio.create(settings.audio.driver);
  ruby::audio.setContext(presentation.viewport.handle());
  ruby::audio.setBlocking(true);
  ruby::audio.setFrequency(48000.0);

  ruby::input.create(settings.input.driver);
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
