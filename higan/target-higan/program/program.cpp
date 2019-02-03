#include "../higan.hpp"
#include "platform.cpp"

Program program;

auto Program::create(shared_pointer<higan::Interface> interface, string location) -> void {
  higan::platform = this;
  emulator = interface;
  emulator->initialize();
  emulator->root()->setProperty("location", location);
  emulator->load();
  viewport.create();
  viewport.show();

  video.create("OpenGL 3.2");
  video.setContext(viewport.context());
  video.setBlocking(false);

  audio.create("OSS");
  audio.setContext(viewport.context());
  audio.setBlocking(false);
  audio.setFrequency(48000.0);

  input.create("SDL");
  input.setContext(viewport.context());

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  if(!viewport.powerToggle.checked()) {
    usleep(20 * 1000);
  } else {
    emulator->run();
  }
}

auto Program::power() -> void {
  emulator->power();
}
