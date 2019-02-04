#include "../higan.hpp"
#include "platform.cpp"

Emulator emulator;

auto Emulator::create(shared_pointer<higan::Interface> instance, string location) -> void {
  higan::platform = this;
  interface = instance;

  Instances::systemManager.construct();
  Instances::inputManager.construct();

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
  for(auto& node : root->find<higan::Node::Peripheral>()) load(node);

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

  //this call will bind all inputs on account of InputManager::devices currently being empty
  inputManager.poll();
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
  for(auto& node : root->find<higan::Node::Peripheral>()) save(node);

  viewports.reset();
  audio.reset();
  inputManager.reset();
  Instances::inputManager.destruct();
  Instances::systemManager.destruct();
  root = {};
  interface->terminate();
  interface.reset();
  interfaces.reset();
  Application::exit();
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

//used to prevent connecting the same (emulated) physical device to multiple ports simultaneously
auto Emulator::connected(string location) -> bool {
  for(auto& peripheral : root->find<higan::Node::Peripheral>()) {
    if(location == peripheral->property("location")) return true;
  }
  return false;
}

auto Emulator::load(higan::Node::Peripheral node) -> void {
  if(auto location = node->property("location")) {
    if(auto document = file::read({location, "node.bml"})) {
      node->copy(higan::Node::unserialize(document));
    }
  }
}

auto Emulator::save(higan::Node::Peripheral node) -> void {
  if(auto location = node->property("location")) {
    file::write({location, "node.bml"}, higan::Node::serialize(node));
  }
}
