#include "../higan.hpp"
#include "port-configuration.cpp"
#include "port-selection.cpp"

namespace Instances { Instance<SystemManager> systemManager; }
SystemManager& systemManager = Instances::systemManager();

SystemManager::SystemManager() {
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
  configureButton.setText("Configure ...").onActivate([&] {
    eventConfigure();
  });

  statusBar.setFont(Font().setBold()).setVisible(false);

  onClose([&] {
    program.quit();
  });

  setSize({600, 320});
}

auto SystemManager::show() -> void {
  root = emulator->root();
  refresh();
  setTitle(emulator->information().name);
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
  print(root->serialize(), "\n");
}

template<typename T> auto SystemManager::attach(T parent, higan::Node node) -> void {
  TreeViewItem item{&parent};
  item.setProperty("path", node->path());

  if(auto boolean = node->cast<higan::Node::Setting::Boolean>()) {
    item.setText({boolean->name, ": ", boolean->value()});
    return;
  }

  if(auto port = node->cast<higan::Node::Port>()) {
    if(auto connected = port->connected()) {
      item.setText({port->name, ": ", connected->name});
      for(auto& child : *connected) {
        attach(item, child);
      }
    } else {
      item.setText(port->name).setForegroundColor({160, 160, 160});
      for(auto& child : *port) {
        attach(item, child);
      }
    }
    return;
  }

  item.setText(node->name);
  for(auto& child : *node) {
    attach(item, child);
  }
}

auto SystemManager::eventChange() -> void {
  if(auto item = connectionList.selected()) {
    if(auto path = item.property("path")) {
      if(auto node = root->find(item.property("path"))) {
        if(node->cast<higan::Node::Port>()) {
          configureButton.setEnabled(true);
          return;
        }
      }
    }
  }
  configureButton.setEnabled(false);
}

auto SystemManager::eventActivate() -> void {
  if(auto item = connectionList.selected()) {
    if(auto path = item.property("path")) {
      if(auto node = root->find(item.property("path"))) {
        if(auto boolean = node->cast<higan::Node::Setting::Boolean>()) {
          boolean->setValue(!boolean->value());
          return refresh();
        }

        if(auto port = node->cast<higan::Node::Port>()) {
          if(!port->property("location") && !port->property("templates")) return eventConfigure();
          if(portSelection.select(port)) refresh();
          return;
        }
      }
    }
  }
}

auto SystemManager::eventConfigure() -> void {
  if(auto item = connectionList.selected()) {
    if(auto path = item.property("path")) {
      if(auto node = root->find(item.property("path"))) {
        if(auto port = node->cast<higan::Node::Port>()) {
          if(portConfiguration.configure(port)) refresh();
          return;
        }
      }
    }
  }
}
