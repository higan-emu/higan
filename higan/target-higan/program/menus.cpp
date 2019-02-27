ActionMenu::ActionMenu(MenuBar* parent) : Menu(parent) {
  setText("System");
  create.setIcon(Icon::Action::Add).setText("Create").onActivate([&] {
    systemManager.eventCreate();
  });
  rename.setIcon(Icon::Application::TextEditor).setText("Rename").onActivate([&] {
    systemManager.eventRename();
  });
  remove.setIcon(Icon::Action::Remove).setText("Delete").onActivate([&] {
    systemManager.eventRemove();
  });
  quit.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { emulator.quit(); });
}

//

SystemMenu::SystemMenu(MenuBar* parent) : Menu(parent) {
  setText("System");
  power.setText("Power").onToggle([&] { emulator.power(power.checked()); });
}

//

SettingsMenu::SettingsMenu(MenuBar* parent) : Menu(parent) {
  setText("Settings");
  video.setText("Video").setIcon(Icon::Device::Display).onActivate([&] {
    programWindow.show(videoSettings);
  });
  audio.setText("Audio").setIcon(Icon::Device::Speaker).onActivate([&] {
    programWindow.show(audioSettings);
  });
  input.setText("Input").setIcon(Icon::Device::Joypad).onActivate([&] {
    programWindow.show(inputSettings);
  });
  showComponents.setText("Show Components").onToggle([&] {
    if(systemOverview.visible()) systemOverview.refresh();
    if(nodeManager.visible()) nodeManager.refresh();
  });
  showHacks.setText("Show Hacks").onToggle([&] {
    if(systemOverview.visible()) systemOverview.refresh();
    if(nodeManager.visible()) nodeManager.refresh();
  });
}

//

ToolsMenu::ToolsMenu(MenuBar* parent) : Menu(parent) {
  setText("Tools");
  saveStateMenu.setText("Save State");
    saveState1.setText("Slot 1").onActivate([&] { emulator.saveState(1); });
    saveState2.setText("Slot 2").onActivate([&] { emulator.saveState(2); });
    saveState3.setText("Slot 3").onActivate([&] { emulator.saveState(3); });
    saveState4.setText("Slot 4").onActivate([&] { emulator.saveState(4); });
    saveState5.setText("Slot 5").onActivate([&] { emulator.saveState(5); });
  loadStateMenu.setText("Load State");
    loadState1.setText("Slot 1").onActivate([&] { emulator.loadState(1); });
    loadState2.setText("Slot 2").onActivate([&] { emulator.loadState(2); });
    loadState3.setText("Slot 3").onActivate([&] { emulator.loadState(3); });
    loadState4.setText("Slot 4").onActivate([&] { emulator.loadState(4); });
    loadState5.setText("Slot 5").onActivate([&] { emulator.loadState(5); });
}

//

HelpMenu::HelpMenu(MenuBar* parent) : Menu(parent) {
  setText("Help");
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
    .setAlignment(programWindow).show();
  });
}
