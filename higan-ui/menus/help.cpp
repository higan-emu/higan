HelpMenu::HelpMenu(MenuBar* parent) : Menu(parent) {
  setText("Help");
  about.setIcon(Icon::Prompt::Question).setText("About higan ...").onActivate([&] {
    image logo{Resource::Higan::Logo};
    logo.shrink();
    AboutDialog()
    .setLogo(logo)
    .setDescription("higan — an advanced multi-system emulator")
    .setVersion(higan::Version)
    .setCopyright(higan::Copyright)
    .setLicense(higan::License, higan::LicenseURI)
    .setWebsite(higan::Website, higan::WebsiteURI)
    .setAlignment(program, {0.5f, program.panelLayout.visible() ? 0.32f : 0.5f})
    .show();
  });
}
