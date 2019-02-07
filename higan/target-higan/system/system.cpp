#include "../higan.hpp"
#include "overview.cpp"
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
    .setAlignment(*this).show();
  });

  changePathAction.setIcon(Icon::Action::Open).setText("Change Path ...").onActivate([&] { eventChangePath(); });

  layout.setPadding(5);
  connectionList.setBackgroundColor(Theme::BackgroundColor);
  connectionList.setForegroundColor(Theme::ForegroundColor);
  connectionList.onChange([&] { eventChange(); });
  connectionList.onContext([&] { eventContext(); });
  resizeGrip.onActivate([&] {
    resizeGrip.setProperty<float>("width", connectionLayout.geometry().width());
  });
  resizeGrip.onResize([&](auto offset) {
    float min = 125, max = layout.geometry().width() - 125;
    float width = resizeGrip.property<float>("width") + offset;
    width = width < min ? min : width > max ? max : width;
    if(layout.cell(connectionLayout).size().width() != width) {
      layout.cell(connectionLayout).setSize({width, ~0});
      layout.resize();
    }
  });
  resizeSpacer.setVisible(false);
  overview.setCollapsible();
  inputMapping.setCollapsible().setVisible(false);
  portSelection.setCollapsible().setVisible(false);
  spacerButton.setVisible(false);

  onClose([&] {
    root = {};
    emulator.quit();
  });

  setSize({720, 360});
}

auto SystemManager::show() -> void {
  root = interface->root();
  refresh();
  setTitle(emulator.system.name);
  setAlignment(Alignment::Center);
  setVisible();
  setFocused();
}

auto SystemManager::refresh() -> void {
  connectionList.reset();
  connectionList.append(TreeViewItem().setText(interface->name()));
  for(auto& node : *root) attach(connectionList, node);
  connectionList.expand().doChange();
}

auto SystemManager::selected() -> higan::Node::Object {
  if(auto item = connectionList.selected()) {
    return item.property<higan::Node::Object>("node");
  }
  return {};
}

template<typename T> auto SystemManager::attach(T parent, higan::Node::Object node, uint depth) -> void {
  if(node->cast<higan::Node::Input>()) return;

  TreeViewItem item{&parent};
  item.setProperty<higan::Node::Object>("node", node);

  image icon;
  icon.allocate(16, 16);
  icon.fill(0x00000000);
  for(uint y = 2; y < 12; y++) {
    auto data = icon.data() + y * icon.pitch() + 6 * icon.stride();
    icon.write(data, 0xff8f8f8f);
  }
  for(uint x = 6; x < 16; x++) {
    auto data = icon.data() + 12 * icon.pitch() + x * icon.stride();
    icon.write(data, 0xff8f8f8f);
  }
  item.setIcon(icon);

  if(auto boolean = node->cast<higan::Node::Boolean>()) {
    item.setText({boolean->name, ": ", boolean->value()});
    return;
  }

  auto name = node->property("name");
  item.setText(name ? name : node->name);
  for(auto& child : *node) {
    attach(item, child, depth + 1);
  }
}

auto SystemManager::eventChange() -> void {
  overview.hide();
  inputMapping.hide();
  portSelection.hide();
  if(auto node = selected()) {
    if(auto nodes = node->find<higan::Node::Input>()) {
      if(nodes.first()->parent == node) inputMapping.show(node);
    }
    if(auto port = node->cast<higan::Node::Port>()) {
      portSelection.show(node);
    }
  }
  if(!inputMapping.visible() && !portSelection.visible()) overview.show();
  layout.setGeometry(layout.geometry());
}

auto SystemManager::eventChangePath() -> void {
  if(auto node = selected()) {
    if(auto port = node->cast<higan::Node::Port>()) {
      auto path = port->property("location");
      if(!path) path = {emulator.system.data, port->type, "/"};
      if(auto location = BrowserDialog()
      .setTitle({port->name, " Path"})
      .setPath(path)
      .setAlignment(*this)
      .selectFolder()
      ) {
        port->setProperty("location", location);
        portSelection.show(node);
      }
    }
  }
}

auto SystemManager::eventContext() -> void {
  if(auto node = selected()) {
    if(auto port = node->cast<higan::Node::Port>()) {
      changePathAction.setVisible();
      contextMenu.setVisible();
    }
  }
}

auto SystemManager::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  inputMapping.eventInput(device, group, input, oldValue, newValue);
}
