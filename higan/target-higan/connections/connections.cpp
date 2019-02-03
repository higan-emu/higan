#include "../higan.hpp"
#include "connection.cpp"

namespace Instances { Instance<ConnectionManager> connectionManager; }
ConnectionManager& connectionManager = Instances::connectionManager();

ConnectionManager::ConnectionManager() {
  layout.setPadding(5);
  connectionList.setBackgroundColor(Theme::BackgroundColor);
  connectionList.setForegroundColor(Theme::ForegroundColor);
  connectionList.onActivate([&] {
    eventActivate();
  });
  configureButton.setEnabled(false).setText("Configure ...");

  setDismissable();
  setSize({480, 280});
}

auto ConnectionManager::show(Window parent) -> void {
  refresh();
  setTitle({emulator->information().name, " Connections"});
  setCentered(parent);
  setVisible();
  setFocused();
//setModal();
}

auto ConnectionManager::refresh() -> void {
  connectionList.reset();
  attach(connectionList, emulator->root());
  connectionList.expand();
}

template<typename T> auto ConnectionManager::attach(T parent, higan::Node node) -> void {
  TreeViewItem item{&parent};
  item.setProperty("nodeID", node->id);
  if(!node->edge || !node->first()) {
    item.setText(node->name);
    if(node->edge) item.setForegroundColor({160, 160, 160});
    for(auto& child : node->nodes) {
      attach(item, child);
    }
  } else {
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
      if(auto leaf = connectionDialog.connect(node)) {
        node->assign(leaf);
        refresh();
        print(emulator->root()->serialize());
      }
    }
  }
}
