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
  port = {};
}

auto NodeManager::refresh() -> void {
  root = interface->root();
  port = {};

  nodeList.reset();
  ListViewItem item{&nodeList};
  item.setProperty<higan::Node::Object>("node", root);
  item.setText(root->name);
  for(auto& node : *root) scan(node);
  nodeList.resize().doChange();
}

auto NodeManager::scan(higan::Node::Object node, uint depth) -> void {
  if(node->is<higan::Node::Input>()) return;
  if(node->is<higan::Node::Component>() && !settingsMenu.showComponents.checked()) return;
  if(node->is<higan::Node::Settings>() && node->name == "Hacks" && !settingsMenu.showHacks.checked()) return;

  ListViewItem item{&nodeList};
  item.setProperty<higan::Node::Object>("node", node);
  string name;
  for(uint n : range(depth)) name.append("   ");
  name.append(node->property("name") ? node->property("name") : node->name);
  if(auto setting = node->cast<higan::Node::Setting>()) {
    name.append(": ", setting->getValue());
  }

  item.setText(name);
  for(auto& node : *node) scan(node, depth + 1);
}

auto NodeManager::eventChange() -> void {
  if(auto node = nodeList.selected().property<higan::Node::Object>("node")) {
    if(auto port = node->cast<higan::Node::Port>()) {
      portConnector.refresh(port);
      return programWindow.show(portConnector);
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
