#include "../higan.hpp"

namespace Instances { Instance<ConnectionManager> connectionManager; }
ConnectionManager& connectionManager = Instances::connectionManager();

ConnectionManager::ConnectionManager() {
  layout.setPadding(5);
  connectionList.setBackgroundColor(Theme::BackgroundColor);
  connectionList.setForegroundColor(Theme::ForegroundColor);
  configureButton.setText("Configure ...");

  setDismissable();
  setSize({480, 280});
}

auto ConnectionManager::show(Window parent) -> void {
  refresh();
  setTitle({emulator->information().name, " Connections"});
  setCentered(parent);
  setVisible();
  setFocused();
  setModal(parent == configurationManager);
}

auto ConnectionManager::refresh() -> void {
  connectionList.reset();
  attach(connectionList, emulator->enumerate());
  connectionList.expand();
}

template<typename T> auto ConnectionManager::attach(T parent, const higan::Interface::Object& object) -> void {
  TreeViewItem item{&parent};
  if(object.type == "System") {
    item.setIcon(Icon::Place::Server);
  } else if(object.type == "Controller") {
    item.setIcon(Icon::Device::Joypad);
  } else {
    item.setIcon(Icon::Prompt::Question);
  }
  item.setText(object.name);
  for(auto& connector : object.connectors) {
    TreeViewItem node{&item};
    node.setText(connector.name);
    if(auto object = connector.connected) {
      node.setIcon(Icon::Action::Add);
      attach(node, *object);
    } else {
      node.setIcon(Icon::Action::Remove);
    }
  }
}
