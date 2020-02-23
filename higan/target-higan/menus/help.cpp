HelpMenu::HelpMenu(MenuBar* parent) : Menu(parent) {
  setText("Help");
  webpage.setIcon(Icon::Application::Browser).setText("Webpage ...").onActivate([&] {
    invoke("https://byuu.org/higan");
  });
  userGuide.setIcon(Icon::Application::Browser).setText("User Guide ...").onActivate([&] {
    invoke("https://byuu.org/higan/user-guide");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setCopyright(higan::Copyright)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setAlignment(program, {0.5f, program.panelLayout.visible() ? 0.32f : 0.5f})
    .show();
  });
}
