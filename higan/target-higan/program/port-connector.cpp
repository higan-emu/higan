PortConnector::PortConnector(View* parent) : PanelItem(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);

  renameAction.setIcon(Icon::Application::TextEditor).setText("Rename ...").onActivate([&] { eventRename(); });
  removeAction.setIcon(Icon::Action::Remove).setText("Delete ...").onActivate([&] { eventRemove(); });

  locationLabel.setMouseCursor(MouseCursor::Hand);
  locationLabel.setFont(Font().setBold());
  locationLabel.setForegroundColor({0, 0, 240});
  locationLabel.onMousePress([&](auto button) {
    if(button == Mouse::Button::Left) eventBrowse();
  });
  peripheralList.onActivate([&] { eventActivate(); });
  peripheralList.onChange([&] { eventChange(); });
  peripheralList.onContext([&] { eventContext(); });
  importButton.setText("Import ...").onActivate([&] { eventImport(); });
  acceptButton.setText("Create").onActivate([&] { eventActivate(); });
}

auto PortConnector::show() -> void {
  importButton.setVisible(isIcarusType());
  setVisible(true);
}

auto PortConnector::hide() -> void {
  setVisible(false);
  port = {};
}

auto PortConnector::isIcarusType() const -> bool {
  if(!port) return false;  //should never occur
  if(port->type() == "Cartridge") return true;
  if(port->type() == "Floppy Disk") return true;
  if(port->type() == "Compact Disc") return true;
  return false;
}

auto PortConnector::refresh(higan::Node::Port port) -> void {
  this->port = port;

  auto path = port->attribute("path");
  //is this the first time refreshing this port?
  if(!path) {
    //see if there's a shared icarus path for it
    if(isIcarusType()) {
      path = {Path::user(), "Emulation/", port->family(), "/"};
    }
    //if not, use a local path for it
    if(!directory::exists(path)) {
      path = {emulator.system.data, port->family(), " ", port->type(), "/"};
    }
    port->setAttribute("path", path);
  }

  //if this is a user path, replace the home directory with ~ for brevity
  if(path.beginsWith(Path::user())) {
    locationLabel.setText(string{path}.trimLeft(Path::user(), 1L).prepend("~/"));
  } else {
    locationLabel.setText(path);
  }

  peripheralList.reset();
  ListViewItem item{&peripheralList};
  item.setAttribute("type", "nothing");
  item.setIcon(Icon::Action::Remove).setText("Nothing");

  if(string location = {emulator.system.templates, port->type(), "/"}) {
    for(auto& name : directory::folders(location)) {
      ListViewItem item{&peripheralList};
      item.setAttribute("type", "template");
      item.setAttribute("location", {location, name});
      item.setAttribute("path", location);
      item.setAttribute("name", name.trimRight("/", 1L));
      item.setIcon(Icon::Action::Add).setText(name);
    }
  }

  for(auto& name : directory::folders(path)) {
    ListViewItem item{&peripheralList};
    item.setAttribute("type", "peripheral");
    item.setAttribute("location", {path, name});
    item.setAttribute("path", path);
    item.setAttribute("name", name.trimRight("/", 1L));
    item.setIcon(Icon::Emblem::Folder).setText(name);
  }

  peripheralList.doChange();
}

auto PortConnector::eventImport() -> void {
  if(auto location = execute("icarus", "--system", interface->name(), "--import").output) {
    refresh(port);
  }
}

auto PortConnector::eventActivate() -> void {
  if(auto item = peripheralList.selected()) {
    if(!port->hotSwappable() && emulator.system.power) {
      auto response = MessageDialog()
      .setText("The peripheral currently connected to this port isn't hot-swappable.\n"
               "Removing it anyway may crash the emulated system.\n"
               "What would you like to do?")
      .setTitle("Warning").setAlignment(programWindow).question({"Force", "Power Off", "Cancel"});
      if(response == "Cancel") return;
      if(response == "Power Off") emulator.power(false);
    }

    auto name = item.attribute("name");

    if(item.attribute("type") == "nothing") {
      port->disconnect();
      nodeManager.refresh();
    }

    else if(item.attribute("type") == "template") {
      auto label = NameDialog()
      .setTitle({"Create New ", name})
      .setAlignment(programWindow)
      .create(item.text());
      if(!label) return;

      auto source = item.attribute("location");
      auto target = string{emulator.system.data, port->family(), " ", port->type(), "/", label, "/"};
      if(directory::exists(target)) return (void)MessageDialog()
      .setText("A directory by this name already exists.")
      .setTitle("Error").setAlignment(programWindow).error();

      if(directory::copy(source, target)) {
        auto peripheral = port->allocate();
        peripheral->setName(name);
        peripheral->setAttribute("location", target);
        peripheral->setAttribute("name", label);
        port->connect(peripheral);
        inputManager.bind();  //bind any inputs this peripheral may contain
        nodeManager.refresh();
      }
    }

    else if(item.attribute("type") == "peripheral") {
      auto location = item.attribute("location");
      auto connected = emulator.connected(location);

      //treat selecting the already-connected device as a no-op that aborts the port connection dialog
      if(connected && connected == port) return programWindow.setPanelItem(home);

      if(connected) return (void)MessageDialog()
      .setText({"This peripheral is already connected to another port:\n\n", connected->name()})
      .setTitle("Error").setAlignment(programWindow).error();

      auto peripheral = port->allocate();
      if(auto markup = file::read({location, "settings.bml"})) {
        peripheral = higan::Node::unserialize(markup);
      }
      peripheral->setAttribute("location", location);
      peripheral->setAttribute("name", item.attribute("name"));
      port->connect(peripheral);
      inputManager.bind();  //bind any inputs this peripheral may contain
      nodeManager.refresh();
    }
  }
}

auto PortConnector::eventChange() -> void {
  acceptButton.setVisible(false);

  if(auto item = peripheralList.selected()) {
    if(item.attribute("type") == "nothing") {
      acceptButton.setText("Disconnect").setVisible();
    }

    else if(item.attribute("type") == "template") {
      acceptButton.setText("Create").setVisible();
    }

    else if(item.attribute("type") == "peripheral") {
      acceptButton.setText("Connect").setVisible();
    }
  }

  controlLayout.resize();
}

auto PortConnector::eventContext() -> void {
  if(auto item = peripheralList.selected()) {
    if(item.attribute("type") == "peripheral") {
      contextMenu.setVisible();
    }
  }
}

auto PortConnector::eventBrowse() -> void {
  auto path = port->attribute("path");
  if(auto location = BrowserDialog()
  .setTitle({port->name(), " Location"})
  .setPath(path ? path : Path::user())
  .setAlignment(programWindow).selectFolder()
  ) {
    port->setAttribute("path", location);
    refresh(port);
  }
}

auto PortConnector::eventRename() -> void {
  if(auto item = peripheralList.selected()) {
    if(item.attribute("type") == "peripheral") {
      auto location = item.attribute("location");
      auto path = item.attribute("path");
      auto name = item.attribute("name");
      if(auto rename = NameDialog()
      .setAlignment(programWindow)
      .rename(name)
      ) {
        if(name == rename) return;

        if(directory::exists({path, rename})) return (void)MessageDialog()
        .setText("A directory by this name already exists.")
        .setTitle("Error").setAlignment(programWindow).error();

        if(!directory::rename({path, name}, {path, rename})) return (void)MessageDialog()
        .setText("Failed to rename directory.")
        .setTitle("Error").setAlignment(programWindow).error();

        //the location must be updated if this peripheral is already connected ...
        if(auto connector = emulator.connected(location)) {
          if(auto connected = connector->connected()) {
            connected->setAttribute("location", {path, rename, "/"});
            connected->setAttribute("name", rename);
            //the name will be updated in the node manager, so it must be refreshed:
            nodeManager.refresh();
          }
        }

        return refresh(port);
      }
    }
  }
}

auto PortConnector::eventRemove() -> void {
  if(auto item = peripheralList.selected()) {
    if(item.attribute("type") == "peripheral") {
      auto location = item.attribute("location");
      auto connected = emulator.connected(location);

      if(!port->hotSwappable() && emulator.system.power && emulator.connected(location)) return (void)MessageDialog()
      .setText({"This peripheral is not hot-swappable and is already connected to a port:\n\n", connected->name()})
      .setTitle("Error").setAlignment(programWindow).error();

      if(MessageDialog()
      .setText("Are you really sure you want to delete this peripheral?\n"
               "All data will be permanently lost!")
      .setTitle("Warning").setAlignment(programWindow).question() == "No"
      ) return;

      //must disconnect device before removing it, as disconnect will attempt to save data to disk
      if(connected) {
        port->disconnect();
        nodeManager.refresh();
      }

      if(!directory::remove(location)) return (void)MessageDialog()
      .setText("Failed to remove directory.")
      .setTitle("Error").setAlignment(programWindow).error();

      refresh(port);
    }
  }
}
