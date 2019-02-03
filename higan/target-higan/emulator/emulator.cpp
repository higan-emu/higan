#include "../higan.hpp"
#include "platform.cpp"

Emulator emulator;

auto Emulator::create(shared_pointer<higan::Interface> instance, string location) -> void {
  higan::platform = this;
  interface = instance;

  system = {};
  system.name = Location::base(location).trimRight("/", 1L);
  system.data = location;
  system.templates = {Path::templates, interface->information().name, "/"};

  string configuration = file::read({location, "root.bml"});
  if(!configuration) {
    auto system = higan::Node::System::create();
    system->name = interface->information().name;
    system->setProperty("location", location);
    configuration = higan::Node::serialize(system);
  }

  interface->initialize(configuration);
  root = interface->root();

  systemManager.show();

  audio.create("OSS");
  audio.setContext(systemManager.handle());
  audio.setBlocking(false);
  audio.setFrequency(48000.0);

  inputManager.create();

  Application::onMain({&Emulator::main, this});

  for(auto& display : root->find<higan::Node::Video>()) {
    display->setProperty("viewportID", viewports.size());
    auto viewport = shared_pointer_make<ViewportWindow>();
    viewport->create(display);
    viewports.append(viewport);
  }
}

auto Emulator::main() -> void {
  inputManager.poll();

  if(!system.power) {
    usleep(20 * 1000);
  } else {
    interface->run();
  }
}

auto Emulator::quit() -> void {
  if(auto location = root->property("location")) {
    file::write({location, "root.bml"}, higan::Node::serialize(root));
  }

  viewports.reset();
  audio.reset();
  inputManager.reset();
  Application::kill();
}

auto Emulator::power(bool on) -> void {
  if(system.power = on) {
    for(auto& viewport : viewports) {
      viewport->show(systemManager);
    }
    interface->power();
  } else {
    for(auto& viewport : viewports) {
      viewport->setVisible(false);
    }
  }
}
