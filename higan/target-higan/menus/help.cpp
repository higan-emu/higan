HelpMenu::HelpMenu(MenuBar* parent) : Menu(parent) {
  setText("Help");
  about.setIcon(Icon::Prompt::Question).setText("About higan ...").onActivate([&] {
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
