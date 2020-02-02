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

  systemMenu.setVisible(false);

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
    .setName(Information::Name)
    .setLogo(Resource::Logo)
    .setVersion(Information::Version)
    .setLicense(Information::License)
    .setWebsite(Information::Website)
    .show();
  });

  statusBar.setFont(Font().setBold());

  onClose([&] {
    program.quit();
  });

  setTitle({Information::Name, " v", Information::Version});
  setBackgroundColor({0, 0, 0});
  setSize({640, 480});
  setAlignment(Alignment::Center);
  setVisible();
}

auto Presentation::loadEmulator() -> void {
  systemMenu.setText(emulator->name);
  systemMenu.setVisible();

  MenuItem unload{&systemMenu};
  unload.setText("Unload").setIcon(Icon::Media::Eject).onActivate([&] {
    program.unloadGame();
  });
}

auto Presentation::unloadEmulator() -> void {
  setTitle({Information::Name, " v", Information::Version});
  systemMenu.setVisible(false);
  systemMenu.reset();
}
