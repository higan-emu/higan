NodeManager::NodeManager(View* parent) : Panel(parent, Size{250, ~0}) {
  setCollapsible().setVisible(false);
  nodeList.onChange([&] { eventChange(); });
  spacerButton.setVisible(false);
}

auto NodeManager::show() -> void {
  refresh();
  setVisible(true);
}

auto NodeManager::hide() -> void {
  setVisible(false);
  root = {};
}

auto NodeManager::bind(higan::Node::Object root) -> void {
  this->root = root;
  nodeList.reset();
}

auto NodeManager::refresh() -> void {
  //save the currently selected node to try and reslect it after rebuilding the tree
  higan::Node::Object active;
  if(auto item = nodeList.selected()) active = item.property<higan::Node::Object>("node");

  nodeList.reset();
  ListViewItem item{&nodeList};
  item.setProperty<higan::Node::Object>("node", root);
  item.setProperty<uint>("depth", 0);
  item.setText(root->name);
  for(auto& node : *root) refresh(node);

  if(active) {
    //try and restore the previously selected node
    for(auto& item : nodeList.items()) {
      if(item.property<higan::Node::Object>("node") == active) {
        item.setSelected();
        break;
      }
    }
  }

  nodeList.resize().doChange();
}

auto NodeManager::refresh(higan::Node::Object node, uint depth) -> void {
  if(node->is<higan::Node::Input>()) return;
  if(node->is<higan::Node::Component>() && !settingsMenu.showComponents.checked()) return;
  if(node->is<higan::Node::Settings>() && node->name == "Hacks" && !settingsMenu.showHacks.checked()) return;

  ListViewItem item{&nodeList};
  item.setProperty<higan::Node::Object>("node", node);
  item.setProperty<uint>("depth", depth);
  string name;
  for(uint n : range(depth)) name.append("   ");
  name.append(node->property("name") ? node->property("name") : node->name);
  if(auto setting = node->cast<higan::Node::Setting>()) {
    name.append(": ", setting->readValue());
    if(!setting->dynamic && setting->readLatch() != setting->readValue()) {
      name.append(" (", setting->readLatch(), ")");
    }
  }
  item.setText(name);

  for(auto& node : *node) refresh(node, depth + 1);
}

auto NodeManager::refreshSettings() -> void {
  for(auto& item : nodeList.items()) {
    auto node = item.property<higan::Node::Object>("node");
    if(auto setting = node->cast<higan::Node::Setting>()) {
      auto depth = item.property<uint>("depth");
      string name;
      for(uint n : range(depth)) name.append("   ");
      name.append(node->property("name") ? node->property("name") : node->name);
      name.append(": ", setting->readValue());
      if(!setting->dynamic && setting->readLatch() != setting->readValue()) {
        name.append(" (", setting->readLatch(), ")");
      }
      item.setText(name);
    }
  }
}

auto NodeManager::deselect() -> void {
  if(auto item = nodeList.selected()) {
    item.setSelected(false);
  }
}

auto NodeManager::eventChange() -> void {
  if(auto node = nodeList.selected().property<higan::Node::Object>("node")) {
    if(auto port = node->cast<higan::Node::Port>()) {
      portConnector.refresh(port);
      return programWindow.show(portConnector);
    }

    if(auto setting = node->cast<higan::Node::Setting>()) {
      settingEditor.refresh(setting);
      return programWindow.show(settingEditor);
    }

    if(auto inputs = node->find<higan::Node::Input>()) {
      if(inputs.first()->parent == node) {
        inputMapper.refresh(node);
        return programWindow.show(inputMapper);
      }
    }
  }

  programWindow.show(home);
}
