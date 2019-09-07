#include "../higan.hpp"
#include "platform.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "states.cpp"
#include "status.cpp"
#include "utility.cpp"

Emulator emulator;

auto Emulator::create(shared_pointer<higan::Interface> instance, string location) -> void {
  interface = instance;

  system = {};
  system.name = Location::base(location).trimRight("/", 1L);
  system.data = location;
  system.templates = {Path::templates, interface->name(), "/"};

  string configuration = file::read({location, "settings.bml"});
  if(!configuration) {
    auto system = higan::Node::System::create();
    system->setName(interface->name());
    system->setProperty("location", location);
    configuration = higan::Node::serialize(system);
  }

  //peripherals may have been renamed or deleted since last run; remove them from the configuration now
  auto document = BML::unserialize(configuration);
  for(auto node : document) validateConfiguration(node, document);
  configuration = BML::serialize(document);

  screens.reset();
  streams.reset();
  interface->load(root, configuration);

  nodeManager.bind(root);
  systemMenu.setText(system.name);
  toolsMenu.pauseEmulation.setChecked(false);
  programWindow.show(home);
  programWindow.show(nodeManager);
  programWindow.setTitle(system.name);
  programWindow.setFocused();

  videoUpdate();
  audioUpdate();
  inputManager.bind(root);
  inputManager.poll();

  videoSettings.eventActivate();
  audioSettings.eventActivate();

  power(false);
}

auto Emulator::unload() -> void {
  if(!interface) return;

  power(false);

  if(auto location = root->property("location")) {
    file::write({location, "settings.bml"}, higan::Node::serialize(root));
  }

  inputManager.unbind();

  root = {};
  interface->unload();
  interface.reset();

  programWindow.setTitle({"higan v", higan::Version});
  systemMenu.setText("System");
  setCaption();
}

auto Emulator::main() -> void {
  updateMessage();
  if(Application::modal()) return (void)usleep(20 * 1000);

  inputManager.poll();
  hotkeys.poll();

  if(!interface) return (void)usleep(20 * 1000);

  if(!system.power
  || programWindow.toolsMenu.pauseEmulation.checked()
  || (!programWindow.viewport.focused() && settings.input.unfocused == "Pause")
  ) {
    usleep(20 * 1000);
  } else {
    interface->run();
    if(events.power) power(false);  //system powered itself off
  }
}

auto Emulator::quit() -> void {
  //make quitting feel more responsive
  programWindow.setVisible(false);
  Application::processEvents();

  //stop processing callbacks and timers
  Application::quit();

  unload();

  interfaces.reset();

  videoInstance.reset();
  audioInstance.reset();
  inputInstance.reset();
}

auto Emulator::power(bool on) -> void {
  if(system.power == on) return;
  if(system.power = on) {
    programWindow.setTitle(interface->game());
    videoUpdateColors();
    audioUpdateEffects();
    events = {};
    interface->power();
    //powering on the system latches static settings
    nodeManager.refreshSettings();
    if(settingEditor.visible()) settingEditor.refresh();
    programWindow.viewport.setFocused();
  } else {
    programWindow.setTitle(system.name);
    setCaption();
    videoInstance.clear();
    audioInstance.clear();
    inputInstance.release();
  }
  systemMenu.power.setChecked(on);
  toolsMenu.saveStateMenu.setEnabled(on);
  toolsMenu.loadStateMenu.setEnabled(on);
}

//used to prevent connecting the same (emulated) physical device to multiple ports simultaneously
auto Emulator::connected(string location) -> higan::Node::Port {
  for(auto& peripheral : root->find<higan::Node::Peripheral>()) {
    if(location == peripheral->property("location")) {
      if(auto parent = peripheral->parent()) return parent.acquire();
    }
  }
  return {};
}

auto Emulator::validateConfiguration(Markup::Node node, Markup::Node parent) -> void {
  for(auto property : node.find("property")) {
    if(property["name"].text() != "location") continue;
    auto location = property["value"].text();
    //if the peripheral is missing, remove it from the tree
    if(!directory::exists(location)) parent.remove(node);
  }
  for(auto branch : node) validateConfiguration(branch, node);
}
