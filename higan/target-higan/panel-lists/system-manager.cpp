SystemManager::SystemManager(View* view) : PanelList(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  listView.onActivate([&] { onActivate(); });
  listView.onChange([&] { onChange(); });
  listView.onContext([&] { onContext(); });
  listCreate.setText("Create").setIcon(Icon::Action::Add).onActivate([&] { doCreate(); });
  listRename.setText("Rename").setIcon(Icon::Application::TextEditor).onActivate([&] { doRename(); });
  listRemove.setText("Remove").setIcon(Icon::Action::Remove).onActivate([&] { doRemove(); });
}

auto SystemManager::show() -> void {
  refresh();
  setVisible(true);
}

auto SystemManager::hide() -> void {
  setVisible(false);
  actionMenu.rename.setEnabled(false);
  actionMenu.remove.setEnabled(false);
}

auto SystemManager::refresh() -> void {
  listView.reset();
  listView.doChange();

  auto location = Path::data;
  for(auto& name : directory::folders(location)) {
    auto document = BML::unserialize(file::read({location, name, "/", "manifest.bml"}));
    if(!document) continue;

    auto system = document["system"].text();
    ListViewItem item{&listView};
    item.setAttribute("location", {location, name});
    item.setAttribute("path", location);
    item.setAttribute("name", name.trimRight("/", 1L));
    item.setAttribute("system", system);
    item.setText(name);

    //hignlight any systems found that have not been compiled into this binary
    if(!interfaces.find([&](auto interface) { return interface->name() == system; })) {
      item.setForegroundColor({240, 80, 80});
      //only show the missing systems in advanced mode
      if(!settings.interface.advancedMode) item.remove();
    }
  }
}

auto SystemManager::onActivate() -> void {
  if(auto item = listView.selected()) {
    if(auto system = item.attribute("system")) {
      if(auto index = interfaces.find([&](auto interface) { return interface->name() == system; })) {
        emulator.create(interfaces[*index], {item.attribute("path"), item.attribute("name"), "/"});
      }
    }
  }
}

auto SystemManager::onChange() -> void {
  if(auto item = listView.selected()) {
    systemOverview.refresh();
    programWindow.setPanelItem(systemOverview);
    actionMenu.rename.setEnabled(true);
    actionMenu.remove.setEnabled(true);
  } else {
    programWindow.setPanelItem(home);
    actionMenu.rename.setEnabled(false);
    actionMenu.remove.setEnabled(false);
  }
}

auto SystemManager::onContext() -> void {
  auto selected = (bool)listView.selected();
  listRename.setVisible(selected);
  listRemove.setVisible(selected);
  listMenu.setVisible();
}

auto SystemManager::doCreate() -> void {
  //clear the current selection to indicate that creation isn't related to the currently selected system item
  listView.selectNone();
  programWindow.setPanelItem(systemCreation);
}

auto SystemManager::doRename() -> void {
  if(auto item = listView.selected()) {
    auto path = item.attribute("path");
    auto from = item.attribute("name");
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

auto SystemManager::doRemove() -> void {
  if(auto item = listView.selected()) {
    if(MessageDialog()
    .setTitle({"Delete ", item.attribute("name")})
    .setText("Are you sure you want to permanently delete the selected system?\n"
             "All of its contents will be permanently lost!")
    .setAlignment(programWindow)
    .question() != "Yes") return;
    auto location = string{item.attribute("path"), item.attribute("name"), "/"};
    if(!directory::remove(location)) return (void)MessageDialog()
    .setTitle("Error")
    .setText("Failed to delete directory.")
    .setAlignment(programWindow)
    .error();
    programWindow.setPanelItem(home);  //hide the system overview for the now-deleted system
    refresh();
  }
}
