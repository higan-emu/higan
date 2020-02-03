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
  videoSizeMenu.setText("Size").setIcon(Icon::Emblem::Image);
  //determine the largest multiplier that can be used by the largest monitor found
  uint monitorHeight = 1;
  for(uint monitor : range(Monitor::count())) {
    monitorHeight = max(monitorHeight, Monitor::workspace(monitor).height());
  }
  //generate size menu
  uint multipliers = max(1, monitorHeight / 240);
  for(uint multiplier : range(2, multipliers + 1)) {
    MenuItem item{&videoSizeMenu};
    item.setText({multiplier, "x (", 240 * multiplier, "p)"});
    item.onActivate([=] {
      settings.video.multiplier = multiplier;
      resizeWindow();
    });
  }
  videoSizeMenu.append(MenuSeparator());
  MenuItem centerWindow{&videoSizeMenu};
  centerWindow.setText("Center Window").setIcon(Icon::Place::Settings).onActivate([&] {
    setAlignment(Alignment::Center);
  });
  videoOutputMenu.setText("Output").setIcon(Icon::Emblem::Image);
  videoOutputCenter.setText("Center").onActivate([&] {
    settings.video.output = "Center";
  });
  videoOutputScale.setText("Scale").onActivate([&] {
    settings.video.output = "Scale";
  });
  videoOutputStretch.setText("Stretch").onActivate([&] {
    settings.video.output = "Stretch";
  });
  if(settings.video.output == "Center") videoOutputCenter.setChecked();
  if(settings.video.output == "Scale") videoOutputScale.setChecked();
  if(settings.video.output == "Stretch") videoOutputStretch.setChecked();
  videoAspectCorrection.setText("Aspect Correction").setChecked(settings.video.aspectCorrection).onToggle([&] {
    settings.video.aspectCorrection = videoAspectCorrection.checked();
  });
  videoShaderMenu.setText("Shader").setIcon(Icon::Emblem::Image);
  loadShaders();
  muteAudioSetting.setText("Mute Audio").setChecked(settings.audio.mute).onToggle([&] {
    settings.audio.mute = muteAudioSetting.checked();
  });
  showStatusBarSetting.setText("Show Status Bar").setChecked(settings.general.showStatusBar).onToggle([&] {
    settings.general.showStatusBar = showStatusBarSetting.checked();
    statusBar.setVisible(settings.general.showStatusBar);
    resizeWindow();
  }).doToggle();
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

  toolsMenu.setVisible(false).setText("Tools");
  saveStateMenu.setText("Save State").setIcon(Icon::Media::Record);
  for(uint slot : range(9)) {
    MenuItem item{&saveStateMenu};
    item.setText({"Slot ", 1 + slot}).onActivate([=] {
      program.stateSave(1 + slot);
    });
  }
  loadStateMenu.setText("Load State").setIcon(Icon::Media::Rewind);
  for(uint slot : range(9)) {
    MenuItem item{&loadStateMenu};
    item.setText({"Slot ", 1 + slot}).onActivate([=] {
      program.stateLoad(1 + slot);
    });
  }

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

  resizeWindow();
  setTitle({Information::Name, " v", Information::Version});
  setBackgroundColor({0, 0, 0});
  setAlignment(Alignment::Center);
  setVisible();
}

auto Presentation::resizeWindow() -> void {
  if(fullScreen()) return;
  if(maximized()) setMaximized(false);

  uint multiplier = max(2, settings.video.multiplier);
  uint width = 320;
  uint height = 240;

  setMinimumSize({width * 2, height * 2});
  setSize({width * multiplier, height * multiplier});
}

auto Presentation::loadShaders() -> void {
  videoShaderMenu.reset();
  videoShaderMenu.setEnabled(ruby::video.hasShader());
  if(!ruby::video.hasShader()) return;

  Group shaders;

  MenuRadioItem none{&videoShaderMenu};
  none.setText("None").onActivate([&] {
    settings.video.shader = "None";
    ruby::video.setShader(settings.video.shader);
  });
  shaders.append(none);

  MenuRadioItem blur{&videoShaderMenu};
  blur.setText("Blur").onActivate([&] {
    settings.video.shader = "Blur";
    ruby::video.setShader(settings.video.shader);
  });
  shaders.append(blur);

  auto location = locate("Shaders/");

  if(ruby::video.driver() == "OpenGL 3.2") {
    for(auto shader : directory::folders(location, "*.shader")) {
      if(shaders.objectCount() == 2) videoShaderMenu.append(MenuSeparator());
      MenuRadioItem item{&videoShaderMenu};
      item.setText(string{shader}.trimRight(".shader/", 1L)).onActivate([=] {
        settings.video.shader = {location, shader};
        ruby::video.setShader(settings.video.shader);
      });
      shaders.append(item);
    }
  }

  if(settings.video.shader == "None") none.setChecked();
  if(settings.video.shader == "Blur") blur.setChecked();
  for(auto item : shaders.objects<MenuRadioItem>()) {
    if(settings.video.shader == string{location, item.text(), ".shader/"}) {
      item.setChecked();
    }
  }
}

auto Presentation::loadEmulator() -> void {
  setTitle(Location::prefix(emulator->interface->game()));

  systemMenu.setText(emulator->name);
  systemMenu.setVisible();

  MenuItem reset{&systemMenu};
  reset.setText("Reset").setIcon(Icon::Action::Refresh).onActivate([&] {
    emulator->interface->power();
    program.showMessage("System reset");
  });

  MenuItem unload{&systemMenu};
  unload.setText("Unload").setIcon(Icon::Media::Eject).onActivate([&] {
    program.unloadGame();
  });

  toolsMenu.setVisible(true);
}

auto Presentation::unloadEmulator() -> void {
  setTitle({Information::Name, " v", Information::Version});

  systemMenu.setVisible(false);
  systemMenu.reset();

  toolsMenu.setVisible(false);
}
