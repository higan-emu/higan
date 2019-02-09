ActionMenu::ActionMenu(MenuBar* parent) : Menu(parent) {
  setText("Action");
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
