#include "../higan.hpp"
#include "connection.cpp"

namespace Instances { Instance<ConnectionManager> connectionManager; }
ConnectionManager& connectionManager = Instances::connectionManager();

ConnectionManager::ConnectionManager() {
  systemMenu.setText("System");
  powerToggle.setText("Power").onToggle([&] {
    program.power(powerToggle.checked());
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
    .setParent(*this).show();
  });

  layout.setPadding(5);
  connectionList.setBackgroundColor(Theme::BackgroundColor);
  connectionList.setForegroundColor(Theme::ForegroundColor);
  connectionList.onActivate([&] {
    eventActivate();
  });
  configureButton.setEnabled(false).setText("Configure ...");

  statusBar.setFont(Font().setBold()).setVisible(false);

  onClose([&] {
    program.quit();
  });

  setDismissable();
  setSize({600, 320});
}

auto ConnectionManager::show() -> void {
  refresh();
  setTitle(emulator->information().name);
  setAlignment({0.25, 0.5});
  setVisible();
  setFocused();
}

auto ConnectionManager::refresh() -> void {
  connectionList.reset();
  for(auto& node : emulator->root()->nodes) {
    attach(connectionList, node);
  }
  connectionList.expand();
}

template<typename T> auto ConnectionManager::attach(T parent, higan::Node::Node node) -> void {
  TreeViewItem item{&parent};
  item.setProperty("nodeID", node->id);

  if(auto boolean = node->cast<higan::Node::Setting::Boolean>()) {
    item.setText({node->name, ": ", boolean->value()});
  }

  else if(!node->edge || !node->first()) {
    item.setText(node->name);
    if(node->edge) item.setForegroundColor({160, 160, 160});
    for(auto& child : node->nodes) {
      attach(item, child);
    }
  }

  else {
    auto edge = node->first();
    item.setText({node->name, ": ", edge->name});
    for(auto& child : edge->nodes) {
      attach(item, child);
    }
  }
}

auto ConnectionManager::eventActivate() -> void {
  auto item = connectionList.selected();
  if(!item) return;

  if(auto nodeID = item.property("nodeID")) {
    if(auto node = emulator->node(nodeID.natural())) {
      if(auto boolean = node->cast<higan::Node::Setting::Boolean>()) {
        boolean->setValue(!boolean->value());
        return refresh();
      }

      if(auto port = node->cast<higan::Node::Port::Cartridge>()) {
        if(auto value = connectionDialog.connect(node)) {
          port->connect([&](auto node) {
            node->setProperty("location", value);
          });
          return refresh();
        }
      }

      if(auto port = node->cast<higan::Node::Port::Peripheral>()) {
        if(auto value = connectionDialog.connect(node)) {
          port->connect(port->list[value.natural()]);
          return refresh();
        }
      }
    }
  }
}
