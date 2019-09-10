SystemManager::SystemManager(View* view) : Panel(view, Size{200_sx, ~0}) {
  setCollapsible().setVisible(false);
  systemList.onActivate([&] { eventActivate(); });
  systemList.onChange([&] { eventChange(); });
  systemList.onContext([&] { eventContext(); });
  systemListCreate.setText("Create").setIcon(Icon::Action::Add).onActivate([&] { eventCreate(); });
  systemListRename.setText("Rename").setIcon(Icon::Application::TextEditor).onActivate([&] { eventRename(); });
  systemListRemove.setText("Delete").setIcon(Icon::Action::Remove).onActivate([&] { eventRemove(); });
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
  actionMenu.rename.setEnabled(false);
  actionMenu.remove.setEnabled(false);

  auto location = Path::data;
  for(auto& name : directory::folders(location)) {
    auto document = BML::unserialize(file::read({location, name, "/", "manifest.bml"}));
    if(!document) continue;

    auto system = document["system"].text();

    ListViewItem item{&systemList};
    item.setAttribute("location", {location, name});
    item.setAttribute("path", location);
    item.setAttribute("name", name.trimRight("/", 1L));
    item.setAttribute("system", system);
    item.setText(name);

    //highlight any systems found that have not been compiled into this binary.
    bool found = false;
    interfaces.foreach([&](auto interface) {
      if(interface->name() == system) found = true;
    });
    if(!found) {
      item.setForegroundColor({240, 80, 80});
      //only show the missing systems in advanced mode.
      if(!settings.interface.advancedMode) item.remove();
    }
  }
}

auto SystemManager::deselect() -> void {
  if(auto item = systemList.selected()) {
    item.setSelected(false);
  }
}

auto SystemManager::eventActivate() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = item.attribute("system")) {
      if(auto index = interfaces.find([&](auto interface) { return interface->name() == system; })) {
        emulator.create(interfaces[*index], {item.attribute("path"), item.attribute("name"), "/"});
      }
    }
  }
}

auto SystemManager::eventChange() -> void {
  if(auto item = systemList.selected()) {
    if(auto system = item.attribute("system")) {
      actionMenu.rename.setEnabled(true);
      actionMenu.remove.setEnabled(true);
      systemOverview.refresh();
      return programWindow.show(systemOverview);
    }
  }

  actionMenu.rename.setEnabled(false);
  actionMenu.remove.setEnabled(false);
  programWindow.show(home);
}

auto SystemManager::eventContext() -> void {
  auto selected = (bool)systemList.selected();
  systemListRename.setVisible(selected);
  systemListRemove.setVisible(selected);
  systemListMenu.setVisible();
}

auto SystemManager::eventCreate() -> void {
  //clear the current selection to indicate that creation affects the entire list,
  //and not just whatever may be currently selected.
  systemList.selectNone();
  programWindow.show(systemCreation);
}

auto SystemManager::eventRename() -> void {
  if(auto item = systemList.selected()) {
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

auto SystemManager::eventRemove() -> void {
  if(auto item = systemList.selected()) {
    if(MessageDialog()
    .setTitle({"Delete ", item.attribute("name")})
    .setText("Are you sure you want to delete the selected item?\n"
             "All of its contents will be permanently lost!")
    .setAlignment(programWindow)
    .question() == "No") return;
    auto location = string{item.attribute("path"), item.attribute("name"), "/"};
    if(!directory::remove(location)) return (void)MessageDialog()
    .setTitle("Error")
    .setText("Failed to delete directory.")
    .setAlignment(programWindow)
    .error();
    programWindow.show(home);  //hide the system overview for the now-deleted system
    refresh();
  }
}
