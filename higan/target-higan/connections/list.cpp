ConnectionListDialog::ConnectionListDialog() {
  layout.setPadding(5);
  nodeList.setBackgroundColor(Theme::BackgroundColor);
  nodeList.setForegroundColor(Theme::ForegroundColor);
  nodeList.onActivate([&] {
    eventAccept();
  });
  nodeList.onChange([&] {
    eventListChange();
  });
  acceptButton.setText("Connect").onActivate([&] {
    eventAccept();
  });

  onClose([&] {
    setModal(false);
    setVisible(false);
  });
}

auto ConnectionListDialog::select(higan::Interface::Edge edge) -> higan::Interface::Node {
  this->edge = edge;
  this->node = {};

  nodeList.reset();
  nodeList.append(TableViewColumn().setExpandable());
  for(auto node : edge->list) {
    TableViewItem item{&nodeList};
    TableViewCell cell{&item};
    if(node->name == "None") {
      cell.setIcon(Icon::Action::Close);
    } else if(node->name == "Gamepad" || node->name == "Super Multitap") {
      cell.setIcon(Icon::Device::Joypad);
    } else {
      cell.setIcon(Icon::Device::Mouse);
    }
    cell.setText(node->name);
  }
  nodeList.doChange();

  setTitle({edge->name, " Connection"});
  setSize({320, 240});
  setCentered(connectionManager);
  setVisible();
  setFocused();
  setModal();
  return node;
}

auto ConnectionListDialog::eventAccept() -> void {
  if(auto item = nodeList.selected()) {
    node = edge->list[item.offset()];
  }
  doClose();
}

auto ConnectionListDialog::eventListChange() -> void {
  acceptButton.setEnabled((bool)nodeList.selected());
}
