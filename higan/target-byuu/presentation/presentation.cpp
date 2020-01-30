#include "../byuu.hpp"
namespace Instances { Instance<Presentation> presentation; }
Presentation& presentation = Instances::presentation();

Presentation::Presentation() {
  loadMenu.setText("Load");
  for(auto& emulator : emulators) {
    MenuItem item{&loadMenu};
    item.setIcon(Icon::Place::Server);
    item.setText({emulator->name, " ..."});
    item.onActivate([=] {
      program.loadGame(emulator);
    });
  }
  { MenuSeparator separator{&loadMenu}; }
  { MenuItem quit{&loadMenu};
    quit.setIcon(Icon::Action::Quit);
    quit.setText("Quit");
    quit.onActivate([&] {
      program.quit();
    });
  }

  systemMenu.setText("Nintendo");

  settingsMenu.setText("Settings");
  configurationAction.setText("Configuration ...").setIcon(Icon::Action::Settings).setEnabled(false);

  helpMenu.setText("Help");
  webpageAction.setText("Webpage ...").setIcon(Icon::Application::Browser).onActivate([&] {
    invoke("https://byuu.org/byuu");
  });
  aboutAction.setText("About byuu ...").setIcon(Icon::Prompt::Question).onActivate([&] {
    AboutDialog()
    .setName("byuu")
    .setLogo(Resource::Logo)
    .setVersion("0")
    .setLicense("GPLv3")
    .setWebsite("https://byuu.org")
    .show();
  });

  statusBar.setFont(Font().setBold());

  onClose([&] {
    program.quit();
  });

  setTitle("byuu v0");
  setBackgroundColor({0, 0, 0});
  setSize({640, 480});
  setAlignment(Alignment::Center);
  setVisible();
}
