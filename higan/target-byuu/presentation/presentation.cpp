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
  muteAudioSetting.setText("Mute Audio").setChecked(settings.audio.mute).onToggle([&] {
    settings.audio.mute = muteAudioSetting.checked();
  });
  videoSettingsAction.setText("Video ...").setIcon(Icon::Device::Display).onActivate([&] {
    settingsWindow.show("Video");
  });
  audioSettingsAction.setText("Audio ...").setIcon(Icon::Device::Speaker).onActivate([&] {
    settingsWindow.show("Audio");
  });
  inputSettingsAction.setText("Input ...").setIcon(Icon::Device::Joypad).onActivate([&] {
    settingsWindow.show("Input");
  });
  hotkeySettingsAction.setText("Hotkeys ...").setIcon(Icon::Device::Keyboard).onActivate([&] {
    settingsWindow.show("Hotkeys");
  });
  driverSettingsAction.setText("Drivers ...").setIcon(Icon::Place::Settings).onActivate([&] {
    settingsWindow.show("Drivers");
  });

  helpMenu.setText("Help");
  webpageAction.setText("Webpage ...").setIcon(Icon::Application::Browser).onActivate([&] {
    invoke("https://byuu.org/byuu");
  });
  aboutAction.setText("About byuu ...").setIcon(Icon::Prompt::Question).onActivate([&] {
    AboutDialog()
    .setName("byuu")
    .setLogo(Resource::Logo)
    .setVersion("0.1")
    .setLicense("GPLv3")
    .setWebsite("https://byuu.org")
    .show();
  });

  statusBar.setFont(Font().setBold());

  onClose([&] {
    program.quit();
  });

  setTitle("byuu v0.1");
  setBackgroundColor({0, 0, 0});
  setSize({640, 480});
  setAlignment(Alignment::Center);
  setVisible();
}
