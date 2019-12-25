NodeManager::NodeManager(View* view) : PanelList(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  listView.onChange([&] { onChange(); });
}

auto NodeManager::show() -> void {
  root = emulator.root;
  listView.selectNone().doChange();
  refresh();
  setVisible(true);
}

auto NodeManager::hide() -> void {
  setVisible(false);
  root = {};
}

//recreate the tree after nodes have been added or removed
auto NodeManager::refresh() -> void {
  //save the currently selected node to try and reselect it after rebuilding the tree
  higan::Node::Object selected;
  if(auto item = listView.selected()) {
    selected = item.attribute<higan::Node::Object>("node");
  }

  listView.reset();
  if(root) for(auto& node : *root) refresh(node, 0);

  //try and restore the previously selected node
  for(auto& item : listView.items()) {
    if(item.attribute<higan::Node::Object>("node") == selected) {
      item.setSelected().setFocused();
    }
  }
}

auto NodeManager::refresh(higan::Node::Object node, uint depth) -> void {
  if(node->is<higan::Node::Event>()) return;
  if(node->is<higan::Node::Input>()) return;
  if(node->is<higan::Node::Sprite>()) return;

  ListViewItem item{&listView};
  item.setAttribute<higan::Node::Object>("node", node);
  item.setAttribute<uint>("depth", depth);
  item.setText(name(node, depth));

  for(auto& node : *node) refresh(node, depth + 1);
}

//refresh the tree after settings have changed
auto NodeManager::refreshSettings() -> void {
  for(auto& item : listView.items()) {
    auto node = item.attribute<higan::Node::Object>("node");
    if(auto setting = node->cast<higan::Node::Setting>()) {
      uint depth = item.attribute<uint>("depth");
      item.setText(name(node, depth));
    }
  }
}

//generate a name for the panel list for a given node
//insert spacing based on depth to simulate a TreeView in a ListView widget
auto NodeManager::name(higan::Node::Object node, uint depth) -> string {
  string name;
  for(uint n : range(depth)) name.append("   ");
  name.append(node->attribute("name") ? node->attribute("name") : node->name());
  if(auto setting = node->cast<higan::Node::Setting>()) {
    name.append(": ", setting->readValue());
    if(!setting->dynamic() && setting->readLatch() != setting->readValue()) {
      name.append(" (", setting->readLatch(), ")");
    }
  }
  return name;
}

auto NodeManager::onChange() -> void {
  if(auto node = listView.selected().attribute<higan::Node::Object>("node")) {
    if(auto port = node->cast<higan::Node::Port>()) {
      portConnector.refresh(port);
      return program.setPanelItem(portConnector);
    }

    if(auto setting = node->cast<higan::Node::Setting>()) {
      settingEditor.refresh(setting);
      return program.setPanelItem(settingEditor);
    }

    if(auto inputs = node->find<higan::Node::Input>()) {
      if(inputs.first()->parent() == node) {
        inputMapper.refresh(node);
        return program.setPanelItem(inputMapper);
      }
    }

    if(auto peripheral = node->cast<higan::Node::Peripheral>()) {
      peripheralOverview.refresh(peripheral);
      return program.setPanelItem(peripheralOverview);
    }
  }

  program.setPanelItem(home);
}
