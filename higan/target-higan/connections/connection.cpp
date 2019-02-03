ConnectionDialog::ConnectionDialog() {
  layout.setPadding(5);
  nodeList.setBackgroundColor(Theme::BackgroundColor);
  nodeList.setForegroundColor(Theme::ForegroundColor);
  nodeList.onActivate([&] {
    eventAccept();
  });
  nodeList.onChange([&] {
    eventChange();
  });
  nameLabel.setText("Name:");
  nameValue.setBackgroundColor(Theme::BackgroundColor);
  nameValue.setForegroundColor(Theme::ForegroundColor);
  nameValue.onActivate([&] {
    eventAccept();
  });
  acceptButton.setText("Connect").onActivate([&] {
    eventAccept();
  });

  onClose([&] {
    setModal(false);
    setVisible(false);
  });

  setDismissable();
}

auto ConnectionDialog::connect(higan::Node node) -> higan::Node {
  this->node = node;
  leaf = {};

  nodeList.reset();
  nodeList.append(TableViewColumn().setExpandable());
  if(node->list) {
    for(auto& leaf : node->list) {
      TableViewItem item{&nodeList};
      TableViewCell cell{&item};
      cell.setText(leaf->name);
    }
  } else {
    string location{Path::user(), "Emulation/Super Famicom/"};
    if(node->name.beginsWith("Sufami Turbo")) location = {Path::user(), "Emulation/Sufami Turbo/"};
    for(auto& name : directory::contents(location)) {
      TableViewItem item{&nodeList};
      item.setProperty("location", {location, name});
      TableViewCell cell{&item};
      cell.setIcon(Icon::Emblem::Folder);
      cell.setText(name.trimRight(".sfc/", 1L).trimRight(".st/", 1L));
    }
  }
  nodeList.doChange();

  setTitle({node->name, " Connection"});
  setSize({320, 240});
  setCentered(connectionManager);
  setVisible();
  setFocused();
  setModal();
  return leaf;
}

auto ConnectionDialog::eventAccept() -> void {
  if(auto item = nodeList.selected()) {
    if(node->list) {
      leaf = node->list[item.offset()];
    } else {
      leaf = higan::Node::create();
      leaf->name = item.cell(0).text();
      leaf->setProperty("location", item.property("location"));
    }
  }
  doClose();
}

auto ConnectionDialog::eventChange() -> void {
  if(auto item = nodeList.selected()) {
    nameValue.setText(item.cell(0).text());
    acceptButton.setEnabled(true);
  } else {
    nameValue.setText();
    acceptButton.setEnabled(false);
  }
}
