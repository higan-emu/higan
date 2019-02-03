#include "../higan.hpp"
#include "list.cpp"

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
  setModal();
}

auto ConnectionManager::refresh() -> void {
  connectionList.reset();
  attach(connectionList, emulator->root());
  connectionList.expand();
}

template<typename T> auto ConnectionManager::attach(T parent, higan::Interface::Node node) -> void {
  TreeViewItem item{&parent};
  item.setProperty("nodeID", node->id);
  if(node->type == "System") {
    item.setIcon(Icon::Place::Server);
  } else if(node->type == "Controller") {
    item.setIcon(Icon::Device::Joypad);
  } else {
    item.setIcon(Icon::Prompt::Question);
  }
  item.setText(node->name);
  for(auto& edge : node->edges) {
    TreeViewItem link{&item};
    link.setProperty("edgeID", edge->id);
    link.setText(edge->name);
    if(edge->node) {
      link.setIcon(Icon::Action::Add);
      attach(link, edge->node);
    } else {
      link.setIcon(Icon::Action::Remove);
    }
  }
}

auto ConnectionManager::eventActivate() -> void {
  auto item = connectionList.selected();
  if(!item) return;

  if(auto nodeID = item.property("nodeID")) {
    if(auto node = emulator->node(nodeID.natural())) {
    }
  }

  if(auto edgeID = item.property("edgeID")) {
    if(auto edge = emulator->edge(edgeID.natural())) {
      if(edge->list) {
        if(auto node = listDialog.select(edge)) {
          print(node->name, "\n");
        }
      }
    }
  }
}
