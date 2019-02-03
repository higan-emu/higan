#include "../higan.hpp"
#include "platform.cpp"

Program program;

auto Program::create(shared_pointer<higan::Interface> interface, string location) -> void {
  higan::platform = this;
  emulator = interface;

  string configuration = file::read({location, "root.bml"});
  if(!configuration) {
    auto system = higan::Node::System::create();
    system->name = emulator->information().name;
    system->setProperty("location", location);
    configuration = higan::Node::serialize(system);
  }

  emulator->initialize(configuration);
  print(higan::Node::serialize(emulator->root()), "\n");

  systemManager.show();

  audio.create("OSS");
  audio.setContext(systemManager.handle());
  audio.setBlocking(false);
  audio.setFrequency(48000.0);

  input.create("SDL");
  input.setContext(systemManager.handle());

  Application::onMain({&Program::main, this});

  for(auto& display : emulator->root()->find<higan::Node::Video>()) {
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
    file::write({location, "root.bml"}, higan::Node::serialize(emulator->root()));
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
