#include "../higan.hpp"
#include "input-mapping.cpp"
#include "port-selection.cpp"

namespace Instances { Instance<SystemManager> systemManager; }
SystemManager& systemManager = Instances::systemManager();

SystemManager::SystemManager() {
  systemMenu.setText("System");
  powerToggle.setText("Power").onToggle([&] {
    emulator.power(powerToggle.checked());
  });

  settingsMenu.setText("Settings");

  toolsMenu.setText("Tools");

  helpMenu.setText("Help");
  documentationAction.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan/");
  });
  aboutAction.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setAuthor(higan::Author)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setPlacement(Placement::Center, *this).show();
  });

  layout.setPadding(5);
  connectionList.setBackgroundColor(Theme::BackgroundColor);
  connectionList.setForegroundColor(Theme::ForegroundColor);
  connectionList.onChange([&] {
    eventChange();
  });
  connectionList.onActivate([&] {
    eventActivate();
  });
  inputMappingButton.setCollapsible().setText("Inputs").onActivate([&] {
    if(auto node = selected()) {
      inputMapping.map(node);
    }
  });

  statusBar.setFont(Font().setBold()).setVisible(false);

  onClose([&] {
    root = {};
    emulator.quit();
  });

  setSize({600, 480});
}

auto SystemManager::show() -> void {
  root = interface->root();
  refresh();
  setTitle(emulator.system.name);
  setAlignment({0.25, 0.5});
  setVisible();
  setFocused();
}

auto SystemManager::refresh() -> void {
  connectionList.reset();
  for(auto& node : *root) {
    attach(connectionList, node);
  }
  connectionList.expand().doChange();
//print(higan::Node::serialize(root), "\n");
}

auto SystemManager::selected() -> higan::Node::Object {
  if(auto item = connectionList.selected()) {
    return item.property<higan::Node::Object>("node");
  }
  return {};
}

template<typename T> auto SystemManager::attach(T parent, higan::Node::Object node) -> void {
  if(node->cast<higan::Node::Input>()) return;

  TreeViewItem item{&parent};
  item.setProperty<higan::Node::Object>("node", node);

  if(auto boolean = node->cast<higan::Node::Boolean>()) {
    item.setText({boolean->name, ": ", boolean->value()});
    return;
  }

  auto name = node->property("name");
  item.setText(name ? name : node->name);
  for(auto& child : *node) {
    attach(item, child);
  }
}

auto SystemManager::eventChange() -> void {
  auto node = selected();
  inputMappingButton.setEnabled(node && !(bool)node->cast<higan::Node::Port>() && (bool)node->find<higan::Node::Input>());
}

auto SystemManager::eventActivate() -> void {
  if(auto node = selected()) {
    if(auto boolean = node->cast<higan::Node::Boolean>()) {
      boolean->setValue(!boolean->value());
      return refresh();
    }

    if(auto port = node->cast<higan::Node::Port>()) {
      return portSelection.select(port);
    }

    if(node->find<higan::Node::Input>()) {
      return inputMapping.map(node);
    }
  }
}

auto SystemManager::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  inputMapping.eventInput(device, group, input, oldValue, newValue);
}
