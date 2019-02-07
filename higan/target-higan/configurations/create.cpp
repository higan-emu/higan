ConfigurationCreatePanel::ConfigurationCreatePanel(HorizontalLayout* parent, Size size) : VerticalLayout(parent, size) {
  systemList.setBackgroundColor(Theme::BackgroundColor);
  systemList.setForegroundColor(Theme::ForegroundColor);
  systemList.append(TableViewColumn().setExpandable());
  systemList.onChange([&] { eventChange(); });
  { TableViewItem item{&systemList};
    TableViewCell cell{&item};
    cell.setIcon(Icon::Emblem::Folder);
    cell.setText("Folder");
  }
  for(auto& interface : interfaces) {
    TableViewItem item{&systemList};
    item.setProperty<shared_pointer<higan::Interface>>("interface", interface);
    TableViewCell cell{&item};
    cell.setIcon(Icon::Place::Server);
    cell.setText(interface->name());
  }
  nameLabel.setText("Name:");
  nameValue.onActivate([&] { eventAccept(); });
  acceptButton.setText("Create").onActivate([&] { eventAccept(); });
}

auto ConfigurationCreatePanel::show() -> void {
  systemList.selected().setSelected(false);
  systemList.doChange();
  nameValue.setText();
  setVisible();
  systemList.setFocused();
}

auto ConfigurationCreatePanel::eventAccept() -> void {
  auto name = nameValue.text().strip();
  if(!name) return;
  name.append("/");

  auto location = Path::data;
  if(auto item = configurationManager.configurationList.selected()) location = item.property("location");
  if(directory::exists({location, name})) {
    if(MessageDialog()
      .setTitle("Warning")
      .setText("A directory by this name already exists.\n"
               "Do you wish to delete the existing directory and create a new one?")
      .setAlignment(configurationManager).question() == "No"
    ) return;
    if(!directory::remove({location, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText("Failed to remove previous directory. The location may be read-only.")
      .setAlignment(configurationManager).error();
  }
  if(!directory::create({location, name})) return (void)MessageDialog()
    .setTitle("Error")
    .setText("Failed to create directory. Either the location is read-only, or the name contains invalid characters.")
    .setAlignment(configurationManager).error();

  if(auto interface = systemList.selected().property<shared_pointer<higan::Interface>>("interface")) {
    file::write({location, name, "identity.bml"}, string{
      "system: ", interface->name(), "\n"
    });
    auto root = interface->root();
    root->setProperty("location", {location, name});
    file::write({location, name, "node.bml"}, higan::Node::serialize(root));
  }

  configurationManager.refresh();
}

auto ConfigurationCreatePanel::eventChange() -> void {
  if(auto item = systemList.selected()) {
    if(item.property<shared_pointer<higan::Interface>>("interface")) {
      nameValue.setEnabled(true).setText(item.cell(0).text());
    } else {
      nameValue.setEnabled(true).setText();
    }
    acceptButton.setEnabled(true);
  } else {
    nameValue.setEnabled(false).setText();
    acceptButton.setEnabled(false);
  }
}
