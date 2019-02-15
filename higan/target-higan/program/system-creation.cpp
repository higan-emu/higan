SystemCreation::SystemCreation(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  systemList.onChange([&] { eventChange(); });
  nameLabel.setText("Name:");
  nameValue.onActivate([&] { eventAccept(); });
  createButton.setText("Create").onActivate([&] { eventAccept(); });
}

auto SystemCreation::show() -> void {
  refresh();
  setVisible(true);
}

auto SystemCreation::hide() -> void {
  setVisible(false);
}

auto SystemCreation::refresh() -> void {
  systemList.reset();
  ListViewItem item{&systemList};
  item.setIcon(Icon::Emblem::Folder).setText("Folder");
  for(auto& interface : interfaces) {
    ListViewItem item{&systemList};
    item.setProperty<shared_pointer<higan::Interface>>("interface", interface);
    item.setIcon(Icon::Place::Server).setText(interface->name());
  }
  systemList.doChange();
}

auto SystemCreation::eventChange() -> void {
  if(auto item = systemList.selected()) {
    if(auto interface = item.property<shared_pointer<higan::Interface>>("interface")) {
      nameValue.setText(interface->name());
    } else {
      nameValue.setText();
    }
    nameLabel.setVisible(true);
    nameValue.setVisible(true);
    createButton.setVisible(true);
  } else {
    nameLabel.setVisible(false);
    nameValue.setVisible(false);
    createButton.setVisible(false);
  }
}

auto SystemCreation::eventAccept() -> void {
  auto name = nameValue.text().strip();
  if(!name) return;
  name.append("/");

  auto location = Path::data;
  if(auto item = systemManager.systemList.selected()) {
    location = {item.property("path"), item.property("name"), "/"};
  }
  if(directory::exists({location, name})) {
    if(MessageDialog()
      .setTitle("Warning")
      .setText("A directory by this name already exists.\n"
               "Do you wish to delete the existing directory and create a new one?")
      .setAlignment(programWindow).question() == "No"
    ) return;
    if(!directory::remove({location, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText("Failed to remove previous directory. The location may be read-only.")
      .setAlignment(programWindow).error();
  }
  if(!directory::create({location, name})) return (void)MessageDialog()
    .setTitle("Error")
    .setText("Failed to create directory. Either the location is read-only, or the name contains invalid characters.")
    .setAlignment(programWindow).error();

  if(auto interface = systemList.selected().property<shared_pointer<higan::Interface>>("interface")) {
    file::write({location, name, "metadata.bml"}, string{
      "system: ", interface->name(), "\n"
    });
    auto root = interface->root();
    root->name = systemList.selected().text();
    root->setProperty("location", {location, name});
    file::write({location, name, "node.bml"}, higan::Node::serialize(root));
  }

  systemManager.refresh();
  programWindow.hide(*this);
}
