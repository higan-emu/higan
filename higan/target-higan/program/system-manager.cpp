SystemManager::SystemManager(View* view) : Panel(view, Size{200_sx, ~0}) {
  setCollapsible().setVisible(false);
  systemList.onActivate([&] { eventActivate(); });
  systemList.onChange([&] { eventChange(); });
}

auto SystemManager::show() -> void {
  refresh();
  setVisible(true);
}

auto SystemManager::hide() -> void {
  setVisible(false);
}

auto SystemManager::refresh() -> void {
  systemList.reset();
  auto location = Path::data;
  for(auto& name : directory::folders(location)) {
    auto document = BML::unserialize(file::read({location, name, "/", "metadata.bml"}));
    if(!document) continue;

    ListViewItem item{&systemList};
    item.setProperty("location", {location, name});
    item.setProperty("path", location);
    item.setProperty("name", name.trimRight("/", 1L));
    item.setProperty("system", document["system"].text());
    item.setText(name);
  }
}

auto SystemManager::deselect() -> void {
  if(auto item = systemList.selected()) {
    item.setSelected(false);
  }
}

auto SystemManager::eventActivate() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = item.property("system")) {
      if(auto index = interfaces.find([&](auto interface) { return interface->name() == system; })) {
        emulator.create(interfaces[*index], {item.property("path"), item.property("name"), "/"});
      }
    }
  }
}

auto SystemManager::eventChange() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = item.property("system")) {
      systemOverview.refresh();
      return programWindow.show(systemOverview);
    }
  }
  programWindow.show(home);
}

auto SystemManager::eventCreate() -> void {
  programWindow.show(systemCreation);
}

auto SystemManager::eventRename() -> void {
  if(auto item = systemList.selected()) {
    auto path = item.property("path");
    auto from = item.property("name");
    if(auto name = NameDialog()
    .setAlignment(programWindow)
    .rename(from)
    ) {
      if(name == from) return;
      if(directory::exists({path, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText("A directory with this name already exists.\n"
               "Please choose a unique name.")
      .setAlignment(programWindow)
      .error();
      if(!directory::rename({path, from}, {path, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText("Failed to rename directory.")
      .setAlignment(programWindow)
      .error();
      refresh();
    }
  }
}

auto SystemManager::eventRemove() -> void {
  if(auto item = systemList.selected()) {
    if(MessageDialog()
    .setTitle({"Delete ", item.property("name")})
    .setText("Are you sure you want to delete the selected item?\n"
             "All of its contents will be permanently lost!")
    .setAlignment(programWindow)
    .question() == "No") return;
    auto location = string{item.property("path"), item.property("name"), "/"};
    if(!directory::remove(location)) return (void)MessageDialog()
    .setTitle("Error")
    .setText("Failed to delete directory.")
    .setAlignment(programWindow)
    .error();
    refresh();
  }
}
