ConfigurationCreateDialog::ConfigurationCreateDialog() {
  layout.setPadding(5);
  systemList.setBackgroundColor(Theme::BackgroundColor);
  systemList.setForegroundColor(Theme::ForegroundColor);
  systemList.append(TableViewColumn().setExpandable());
  systemList.onChange([&] { eventListChange(); });
  { TableViewItem item{&systemList};
    TableViewCell cell{&item};
    cell.setIcon(Icon::Emblem::Folder);
    cell.setText("Folder");
  }
  for(auto emulator : interfaces) {
    TableViewItem item{&systemList};
    item.setProperty("system", emulator->name());
    TableViewCell cell{&item};
    cell.setIcon(Icon::Place::Server);
    cell.setText(emulator->name());
  }
  nameLabel.setText("Name:");
  nameValue.onActivate([&] {
    eventAccept();
  });
  acceptButton.setText("Create").onActivate([&] {
    eventAccept();
  });

  onClose([&] {
    setModal(false);
    setVisible(false);
  });

  setDismissable();
  setTitle("Create");
  setSize({360, 325});
}

auto ConfigurationCreateDialog::run() -> Result {
  result = {};
  systemList.selected().setSelected(false);
  systemList.doChange();
  nameValue.setText();
  setPlacement(Placement::Overlap, configurationManager);
  setVisible();
  setFocused();
  systemList.setFocused();
  Application::processEvents();
  layout.setGeometry(layout.geometry());
  setModal();
  return result;
}

auto ConfigurationCreateDialog::eventAccept() -> void {
  string name = nameValue.text().strip();
  if(!name) return (void)MessageDialog()
    .setTitle("Error")
    .setText("The name field cannot be left blank.")
    .setPlacement(Placement::Center, *this).error();
  result.system = systemList.selected().property("system");
  result.name = nameValue.text().strip();
  doClose();
}

auto ConfigurationCreateDialog::eventListChange() -> void {
  if(auto item = systemList.selected()) {
    nameValue.setEditable(true).setText(item.property("system"));
    acceptButton.setEnabled(true);
  } else {
    nameValue.setEditable(false).setText();
    acceptButton.setEnabled(false);
  }
}
