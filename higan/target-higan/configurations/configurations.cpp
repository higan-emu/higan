#include "../higan.hpp"
#include "home.cpp"
#include "create.cpp"
#include "review.cpp"

namespace Instances { Instance<ConfigurationManager> configurationManager; }
ConfigurationManager& configurationManager = Instances::configurationManager();

ConfigurationManager::ConfigurationManager() {
  directory::create(Path::data);

  actionMenu.setText("System");
  launchAction.setIcon(Icon::Emblem::Program).setText("Launch ...").onActivate([&] { reviewPanel.eventLaunch(); });
  createAction.setIcon(Icon::Action::Add).setText("Create ...").onActivate([&] { eventCreate(); });
  renameAction.setIcon(Icon::Application::TextEditor).setText("Rename ...").onActivate([&] { eventRename(); });
  removeAction.setIcon(Icon::Action::Remove).setText("Delete ...").onActivate([&] { eventRemove(); });
  quitAction.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { doClose(); });

  settingsMenu.setText("Settings");

  pathsMenu.setText("Paths");
  dataPathChange.setIcon(Icon::Action::Settings).setText("Change Data Path ...").onActivate([&] {
    if(auto location = BrowserDialog()
    .setTitle("Select Data Path")
    .setPath(Path::data)
    .setAlignment(*this).selectFolder()
    ) {
      Path::data = location;
      file::write({Path::settings, "paths.bml"}, string{
        "data: ", Path::data, "\n",
        "templates: ", Path::templates, "\n"
      });
      refresh();
    }
  });
  dataPathOpen.setIcon(Icon::Action::Open).setText("Open Data Path ...").onActivate([&] { invoke(Path::data); });
  templatesPathOpen.setIcon(Icon::Action::Open).setText("Open Templates Path ...").onActivate([&] { invoke(Path::templates); });
  settingsPathOpen.setIcon(Icon::Action::Open).setText("Open Settings Path ...").onActivate([&] { invoke(Path::settings); });

  helpMenu.setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] { invoke("https://doc.byuu.org/higan/"); });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setAuthor(higan::Author)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setAlignment(*this).show();
  });

  launchPopup.setIcon(Icon::Emblem::Program).setText("Launch ...").onActivate([&] { reviewPanel.eventLaunch(); });
  createPopup.setIcon(Icon::Action::Add).setText("Create ...").onActivate([&] { eventCreate(); });
  renamePopup.setIcon(Icon::Application::TextEditor).setText("Rename ...").onActivate([&] { eventRename(); });
  removePopup.setIcon(Icon::Action::Remove).setText("Delete ...").onActivate([&] { eventRemove(); });

  layout.setPadding(5);
  configurationList.setBackgroundColor(Theme::BackgroundColor);
  configurationList.setForegroundColor(Theme::ForegroundColor);
  resizeGrip.onActivate([&] {
    resizeGrip.setProperty<float>("width", configurationLayout.geometry().width());
  });
  resizeGrip.onResize([&](auto offset) {
    float min = 125, max = layout.geometry().width() - 125;
    float width = resizeGrip.property<float>("width") + offset;
    width = width < min ? min : width > max ? max : width;
    if(layout.cell(configurationLayout).size().width() != width) {
      layout.cell(configurationLayout).setSize({width, ~0});
      layout.resize();
    }
  });
  resizeSpacer.setVisible(false);
  homePanel.setCollapsible();
  createPanel.setCollapsible();
  reviewPanel.setCollapsible();
  configurationList.onActivate([&] { reviewPanel.eventLaunch(); });
  configurationList.onChange([&] { eventChange(); });
  configurationList.onContext([&] { eventContext(); });
  controlLayout.setAlignment(0.5);
  createButton.setText("Create").onActivate([&] { eventCreate(); });
  renameButton.setText("Rename").onActivate([&] { eventRename(); });
  removeButton.setText("Delete").onActivate([&] { eventRemove(); });

  onClose([&] { Application::quit(); });

  setTitle({"higan v", higan::Version});
  setSize({720, 360});
  setAlignment(Alignment::Center);
//setBackgroundColor({255, 255, 240});

  refresh();
}

auto ConfigurationManager::show() -> void {
  setVisible();
  setFocused();
}

auto ConfigurationManager::refresh() -> void {
  configurationList.reset();
  scan(configurationList, Path::data);
  configurationList.expand();
  configurationList.doChange();
}

template<typename T> auto ConfigurationManager::scan(T parent, string location) -> void {
  for(auto& name : directory::folders(location)) {
    TreeViewItem item{&parent};
    item.setProperty("location", {location, name});
    if(auto document = BML::unserialize(file::read({location, name, "identity.bml"}))) {
      item.setProperty("system", document["system"].text());
      item.setIcon(Icon::Place::Server).setText(string{name}.trimRight("/", 1L));
    } else {
      item.setIcon(Icon::Emblem::Folder).setText(string{name}.trimRight("/", 1L));
      scan(item, {location, name});
    }
  }
}

template<typename T> auto ConfigurationManager::attach(T parent, higan::Node::Object node) -> void {
  if(node->cast<higan::Node::Input>()) return;

  TreeViewItem item{&parent};

  auto name = node->property("name");
  item.setText(name ? name : node->name);
  for(auto& child : *node) attach(item, child);
}

auto ConfigurationManager::eventChange() -> void {
  auto item = configurationList.selected();
  auto system = item.property("system");
  createAction.setEnabled(!(bool)system);
  createButton.setEnabled(!(bool)system);
  renameAction.setEnabled((bool)item);
  renameButton.setEnabled((bool)item);
  removeAction.setEnabled((bool)item);
  removeButton.setEnabled((bool)item);
  launchAction.setEnabled((bool)system);

  homePanel.setVisible(false);
  createPanel.setVisible(false);
  reviewPanel.setVisible(false);
  if(system) {
    reviewPanel.refresh(item.property("location"));
    reviewPanel.setVisible();
  } else {
    homePanel.setVisible();
  }
  layout.setGeometry(layout.geometry());
  controlLayout.setEnabled(true);
}

auto ConfigurationManager::eventContext() -> void {
  auto item = configurationList.selected();
  auto system = item.property("system");
  launchPopup.setVisible((bool)system);
  launchPopupSeparator.setVisible((bool)system);
  createPopup.setVisible(!(bool)system);
  renamePopup.setVisible((bool)item);
  removePopup.setVisible((bool)item);
  contextMenu.setVisible();
}

auto ConfigurationManager::eventCreate() -> void {
  controlLayout.setEnabled(false);
  homePanel.setVisible(false);
  reviewPanel.setVisible(false);
  createPanel.show();
  layout.setGeometry(layout.geometry());
}

auto ConfigurationManager::eventRename() -> void {
  auto item = configurationList.selected();
  if(!item) return;
  auto location = item.property("location");
  auto originalName = Location::base(location);
  auto name = NameDialog()
  .setTitle({"Rename ", item.text()})
  .setText("Enter a new name:")
  .setAlignment(*this)
  .rename(item.text());
  if(!name) return;  //user cancelled the operation
  name.append("/");
  if(name == originalName) return;
  location = Location::dir(location);
  if(directory::exists({location, name})) {
    if(MessageDialog()
      .setTitle("Warning")
      .setText("A directory by this name already exists.\n"
               "Do you wish to delete the existing directory and replace it with this one?")
      .setAlignment(*this).question() == "No"
    ) return;
    if(!directory::remove({location, name})) return (void)MessageDialog()
      .setTitle("Error")
      .setText({"Failed to remove previous directory. The location may be read-only.\n",
                "Location: ", location, "\n",
                "Name: ", name})
      .setAlignment(*this).error();
  }
  if(!directory::rename({location, originalName}, {location, name})) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to rename directory. Either the location is read-only, or the name contains invalid characters.\n",
              "Location: ", location, "\n",
              "Name: ", name})
    .setAlignment(*this).error();
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
    .setAlignment(*this).question() == "No"
  ) return;
  if(!directory::remove(location)) return (void)MessageDialog()
    .setTitle("Error")
    .setText({"Failed to remove directory. The location may be read-only.\n",
              "Location: ", location})
    .setAlignment(*this).error();
  refresh();
}
