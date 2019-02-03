PortSelectionDialog::PortSelectionDialog() {
  layout.setPadding(5);
  nodeList.setBackgroundColor(Theme::BackgroundColor);
  nodeList.setForegroundColor(Theme::ForegroundColor);
  nodeList.onChange([&] {
    eventChange();
  });
  nodeList.onActivate([&] {
    eventAccept();
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

auto PortSelectionDialog::select(higan::Node::Port port) -> higan::Node {
  this->port = port;
  result = {};

  nodeList.reset();
  nodeList.append(TableViewColumn().setExpandable());
  if(auto location = port->property("templates")) {
    for(auto& name : directory::folders(location)) {
      TableViewItem item{&nodeList};
      item.setProperty("type", "template");
      item.setProperty("location", {location, name});
      TableViewCell cell{&item};
      cell.setIcon(Icon::Emblem::Program).setText(name.trimRight("/", 1L));
    }
  }
  if(auto location = port->property("location")) {
    for(auto& name : directory::folders(location)) {
      TableViewItem item{&nodeList};
      item.setProperty("type", "object");
      item.setProperty("location", {location, name});
      TableViewCell cell{&item};
      cell.setIcon(Icon::Emblem::Folder).setText(name.trimRight("/", 1L));
    }
  }
  nodeList.doChange();

  setTitle(port->name);
  setSize({480, 400});
  setPlacement(Placement::After, systemManager);
  setVisible();
  setFocused();
  setModal();
  return result;
}

auto PortSelectionDialog::eventChange() -> void {
  if(auto item = nodeList.selected()) {
    bool isTemplate = item.property("type") == "template";
    nameLabel.setVisible(isTemplate);
    nameValue.setText(item.cell(0).text()).setVisible(isTemplate);
    acceptButton.setEnabled(true);
  } else {
    nameLabel.setVisible(false);
    nameValue.setText().setVisible(false);
    acceptButton.setEnabled(false);
  }
}

auto PortSelectionDialog::eventAccept() -> void {
  if(auto item = nodeList.selected()) {
    if(item.property("type") == "template") {
      result = port->connect([&](auto node) {
        node->kind = item.cell(0).text();
        node->name = nameValue.text().strip();
        node->setProperty("location", item.property("location"));
      });
    }
    if(item.property("type") == "object") {
      result = port->connect([&](auto node) {
        node->name = nameValue.text().strip();
        node->setProperty("location", item.property("location"));
      });
    }
  }
  doClose();
}
