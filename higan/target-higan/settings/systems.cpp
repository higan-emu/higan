SystemsAppendWindow::SystemsAppendWindow() {
  layout.setPadding(5);
  nameLabel.setText("Name:");
  TableViewColumn column{&systemsList};
  for(auto emulator : emulators) {
    TableViewItem item{&systemsList};
    item.setProperty("systemID", systemsList.itemCount() - 1);
    item.append(TableViewCell().setText(emulator->information().name));
  }
  systemsList.onChange([&] {
    eventChangeSelection();
  }).doChange();
  nameValue.onChange([&] {
    eventChangeName();
  });
  acceptButton.setText("Append").onActivate([&] {
    eventAccept();
  });

  setTitle("Add System");
  setSize({320, 240});
}

auto SystemsAppendWindow::show() -> void {
  setCentered(settingsWindow);
  setVisible();
  setFocused();
}

auto SystemsAppendWindow::eventChangeSelection() -> void {
  if(auto item = systemsList.selected()) {
    nameValue.setEnabled(true).setText(item.cell(0).text());
    acceptButton.setEnabled(true);
  } else {
    nameValue.setEnabled(false).setText();
    acceptButton.setEnabled(false);
  }
}

auto SystemsAppendWindow::eventChangeName() -> void {
  if(!systemsList.selected() || !nameValue.text().strip()) {
    return (void)acceptButton.setEnabled(false);
  }
  acceptButton.setEnabled(true);
}

auto SystemsAppendWindow::eventAccept() -> void {
  if(!systemsList.selected()) return;
  auto systemID = systemsList.selected().property("systemID");
  if(systemID >= emulators.size()) return;
  auto emulator = emulators[systemID.natural()];

  directory::create(locate("Systems/"));
  auto location = locate({"Systems/", nameValue.text().strip(), ".sys/"});
  if(directory::exists(location)) {
    if(MessageDialog()
      .setText("A system by this name already exists. Overwrite it?")
      .setParent(*this).question() == "No") return;
    directory::remove(location);
  }
  directory::create(location);
  file::write({location, "options.bml"}, emulator->options().defaults());
  file::write({location, "properties.bml"}, emulator->properties().defaults());
  setVisible(false);
  settingsWindow.systems.refresh();
  presentation.refreshSystems();
}

SystemsModifyWindow::SystemsModifyWindow() {
  layout.setPadding(5);
  propertiesEdit.setFont(Font().setFamily(Font::Mono));
  acceptButton.setText("Modify").onActivate([&] {
    eventAccept();
  });

  setTitle("Modify System");
  setSize({400, 320});
}

auto SystemsModifyWindow::show(string systemName) -> void {
  this->systemName = systemName;
  propertiesEdit.setText(file::read(locate({"Systems/", systemName, ".sys/properties.bml"})));

  setCentered(settingsWindow);
  setVisible();
  setFocused();
}

auto SystemsModifyWindow::eventAccept() -> void {
  file::write(locate({"Systems/", systemName, ".sys/properties.bml"}), propertiesEdit.text());
  setVisible(false);
}

SystemsPanel::SystemsPanel(TabFrame* parent) : TabFrameItem(parent) {
  layout.setPadding(5);
  appendButton.setText("Append").onActivate([&] {
    appendWindow.show();
  });
  modifyButton.setText("Modify").onActivate([&] {
    if(auto item = systemsList.selected()) {
      modifyWindow.show(item.cell(0).text());
    }
  });
  removeButton.setText("Remove").onActivate([&] {
  });

  setIcon(Icon::Device::Storage);
  setText("Systems");

  refresh();
}

auto SystemsPanel::refresh() -> void {
  systemsList.reset();
  systemsList.append(TableViewColumn());
  for(auto system : directory::folders(locate("Systems/"), "*.sys")) {
    TableViewItem item{&systemsList};
    item.append(TableViewCell().setText(system.trimRight(".sys/", 1L)));
  }
}
