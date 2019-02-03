#include "../higan.hpp"
#include "platform.cpp"

Program program;

auto Program::create(shared_pointer<higan::Interface> interface, string location) -> void {
  higan::platform = this;

  emulator = interface;
  emulator->initialize([&](auto system) {
    system->setProperty("location", location);
  });
  if(auto document = file::read({location, "root.bml"})) {
    emulator->import(document);
  }
  print(emulator->root()->serialize(), "\n");

  systemManager.show();

  audio.create("OSS");
  audio.setContext(systemManager.handle());
  audio.setBlocking(false);
  audio.setFrequency(48000.0);

  input.create("SDL");
  input.setContext(systemManager.handle());

  Application::onMain({&Program::main, this});

  for(auto& display : emulator->root()->find<higan::Node::Port::Video>()) {
    display->setProperty("viewportID", viewports.size());
    auto viewport = shared_pointer_make<ViewportWindow>();
    viewport->create(display);
    viewports.append(viewport);
  }
}

auto Program::main() -> void {
  if(!system.power) {
    usleep(20 * 1000);
  } else {
    emulator->run();
  }
}

auto Program::quit() -> void {
  if(auto location = emulator->root()->property("location")) {
  //file::write({location, "root.bml"}, emulator->export());
  }

  viewports.reset();
  audio.reset();
  input.reset();
  Application::kill();
}

auto Program::power(bool on) -> void {
  if(system.power = on) {
    for(auto& viewport : viewports) {
      viewport->show(systemManager);
    }
    emulator->power();
  } else {
    for(auto& viewport : viewports) {
      viewport->setVisible(false);
    }
  }
}
