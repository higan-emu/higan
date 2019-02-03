#include "../higan.hpp"
#include "create.cpp"
#include "rename.cpp"
#include "properties.cpp"

namespace Instances { Instance<ConfigurationManager> configurationManager; }
ConfigurationManager& configurationManager = Instances::configurationManager();

ConfigurationManager::ConfigurationManager() {
  directory::create(locate("Configurations/"));

  actionMenu.setText("Action");
  createAction.setIcon(Icon::Action::Add).setText("Create ...").onActivate([&] {
    eventCreate();
  });
  renameAction.setIcon(Icon::Application::TextEditor).setText("Rename ...").onActivate([&] {
    eventRename();
  });
  removeAction.setIcon(Icon::Action::Remove).setText("Delete ...").onActivate([&] {
    eventRemove();
  });
  connectionsAction.setIcon(Icon::Place::Server).setText("Connections ...").onActivate([&] {
    eventConnections();
  });
  propertiesAction.setIcon(Icon::Action::Properties).setText("Properties ...").onActivate([&] {
    eventProperties();
  });
  quitAction.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] {
    Application::kill();
  });
  settingsMenu.setText("Settings");
  helpMenu.setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan/");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setAuthor(higan::Author)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setParent(*this).show();
  });

  layout.setPadding(5);
  configurationList.setBackgroundColor(Theme::BackgroundColor);
  configurationList.setForegroundColor(Theme::ForegroundColor);
  configurationList.onChange([&] {
    eventListChange();
  });
  controlLayout.setAlignment(0.5);
  locationLabel.setText({Path::userData(), "higan"}).setFont(Font().setBold()).setForegroundColor({0, 0, 240}).onMouseRelease([&](auto button) {
    if(button == Mouse::Button::Left) invoke(locationLabel.text());
  });
  createButton.setText("Create").onActivate([&] {
    eventCreate();
  });
  renameButton.setText("Rename").onActivate([&] {
    eventRename();
  });
  removeButton.setText("Delete").onActivate([&] {
    eventRemove();
  });
  controlSpacer.setColor({160, 160, 160});
  connectionsButton.setText("Connections").onActivate([&] {
    eventConnections();
  });
  propertiesButton.setText("Properties").onActivate([&] {
    eventProperties();
  });

  onClose([&] { Application::kill(); });

  setTitle({"higan v", higan::Version});
  setSize({640, 360});
  setCentered();

  refresh();
}

auto ConfigurationManager::show() -> void {
  setVisible();
  setFocused();
}

auto ConfigurationManager::refresh() -> void {
  configurationList.reset();
  scan(configurationList, locate("Configurations/"));
  configurationList.expand();
  configurationList.doChange();
}

template<typename T> auto ConfigurationManager::scan(T parent, string location) -> void {
  for(auto& name : directory::folders(location)) {
    TreeViewItem item{&parent};
    item.setProperty("location", {location, name});
    if(file::exists({location, name, "properties.bml"})) {
      auto document = BML::unserialize(file::read({location, name, "properties.bml"}));
      item.setProperty("system", document["system/name"].text());
      item.setIcon(Icon::Place::Server);
    } else {
      item.setIcon(Icon::Emblem::Folder);
    }
    item.setText(string{name}.trimRight("/", 1L));
    scan(item, {location, name});
  }
}

auto ConfigurationManager::eventListChange() -> void {
  auto item = configurationList.selected();
  auto system = item.property("system");
  renameAction.setEnabled((bool)item);
  renameButton.setEnabled((bool)item);
  removeAction.setEnabled((bool)item);
  removeButton.setEnabled((bool)item);
  connectionsAction.setEnabled((bool)item && (bool)system);
  connectionsButton.setEnabled((bool)item && (bool)system);
  propertiesAction.setEnabled((bool)item && (bool)system);
  propertiesButton.setEnabled((bool)item && (bool)system);
}

auto ConfigurationManager::eventCreate() -> void {
  auto [system, name] = createDialog.run();
  if(!name) return;  //user cancelled the operation
  name.append("/");
  auto location = locate("Configurations/");
  if(auto item = configurationList.selected()) location = item.property("location");
  if(directory::exists({location, name})) {
    if(MessageDialog()
      .setTitle("Warning")
      .setText("A directory by this name already exists.\n"
               "Do you wish to delete the existing directory and create a new one?")
      .setParent(*this).question() == "No"
    ) return;
    if(!directory::remove({location, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText({"Failed to remove previous directory. The location may be read-only.\n",
                "Location: ", location,
                "Name: ", name})
      .setParent(*this).error();
  }
  if(!directory::create({location, name})) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to create directory. Either the location is read-only, or the name contains invalid characters.\n",
              "Location: ", location, "\n",
              "Name: ", name})
    .setParent(*this).error();
  if(system) {
    if(auto index = emulators.find([&](auto emulator) { return emulator->information().name == system; })) {
      file::write({location, name, "properties.bml"}, emulators[*index]->properties().serialize());
    }
  }
  refresh();
}

auto ConfigurationManager::eventRename() -> void {
  auto item = configurationList.selected();
  if(!item) return;
  auto location = item.property("location");
  auto originalName = Location::base(location);
  auto name = renameDialog.run(item.text());
  if(!name) return;  //user cancelled the operation
  name.append("/");
  if(name == originalName) return;
  location = Location::dir(location);
  if(directory::exists({location, name})) {
    if(MessageDialog()
      .setTitle("Warning")
      .setText("A directory by this name already exists.\n"
               "Do you wish to delete the existing directory and replace it with this one?")
      .setParent(*this).question() == "No"
    ) return;
    if(!directory::remove({location, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText({"Failed to remove previous directory. The location may be read-only.\n",
                "Location: ", location, "\n",
                "Name: ", name})
      .setParent(*this).error();
  }
  if(!directory::rename({location, originalName}, {location, name})) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to rename directory. Either the location is read-only, or the name contains invalid characters.\n",
              "Location: ", location, "\n",
              "Name: ", name})
    .setParent(*this).error();
  refresh();
}

auto ConfigurationManager::eventRemove() -> void {
  auto item = configurationList.selected();
  if(!item) return;
  auto location = item.property("location");
  if(MessageDialog()
    .setTitle("Warning")
    .setText({"This will permanently destroy this directory, and all of its contents recursively.\n",
              "All related settings and content will be lost. Are you really sure you want to do this?\n",
              "Location: ", location})
    .setParent(*this).question() == "No"
  ) return;
  if(!directory::remove(location)) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to remove directory. The location may be read-only.\n",
              "Location: ", location})
    .setParent(*this).error();
  refresh();
}

auto ConfigurationManager::eventConnections() -> void {
  auto item = configurationList.selected();
  if(!item || !item.property("system")) return;
  auto system = item.property("system");
  if(auto index = emulators.find([&](auto emulator) { return emulator->information().name == system; })) {
    emulator = emulators[*index];
    emulator->initialize();
    connectionManager.show(*this);
  }
}

auto ConfigurationManager::eventProperties() -> void {
  auto item = configurationList.selected();
  if(!item || !item.property("system")) return;
  auto location = item.property("location");
  auto properties = propertiesDialog.run(item.text(), file::read({location, "properties.bml"}));
  if(!properties) return;  //user cancelled the operation
  if(!file::write({location, "properties.bml"}, properties)) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to write properties.bml file. The location may be read-only.\n"
              "Location: ", location})
    .setParent(*this).error();
}
