PortManager::PortManager(View* view) : PanelList(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  listView.onChange([&] { onChange(); });
}

auto PortManager::show() -> void {
  root = emulator.root;
  listView.selectNone().doChange();
  refresh();
  setVisible(true);
}

auto PortManager::hide() -> void {
  setVisible(false);
  root = {};
}

auto PortManager::refresh() -> void {
  //save the currently selected node to try and reselect it after rebuilding the tree
  higan::Node::Object selected;
  if(auto item = listView.selected()) {
    selected = item.attribute<higan::Node::Object>("node");
  }

  listView.reset();
  if(root) for(auto& node : *root) refresh(node);

  //try and restore the previously selected node
  for(auto& item : listView.items()) {
    if(item.attribute<higan::Node::Object>("node") == selected) {
      item.setSelected().setFocused();
    }
  }
}

auto PortManager::refresh(higan::Node::Object node) -> void {
  if(auto port = node->cast<higan::Node::Port>()) {
    ListViewItem item{&listView};
    item.setAttribute<higan::Node::Object>("node", port);
    item.setText(port->attribute("name") ? port->attribute("name") : port->name());
    if(auto peripheral = port->connected()) {
      ListViewItem item{&listView};
      item.setAttribute<higan::Node::Object>("node", peripheral);
      item.setText({"   ", peripheral->attribute("name") ? peripheral->attribute("name") : peripheral->name()});
    }
  }

  for(auto& node : *node) refresh(node);
}

auto PortManager::onChange() -> void {
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
