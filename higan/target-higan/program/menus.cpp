ActionMenu::ActionMenu(MenuBar* parent) : Menu(parent) {
  setText("System");
  create.setIcon(Icon::Action::Add).setText("Create").onActivate([&] {
    programWindow.showPanels();
    systemManager.eventCreate();
  });
  rename.setIcon(Icon::Application::TextEditor).setText("Rename").setEnabled(false).onActivate([&] {
    systemManager.eventRename();
  });
  remove.setIcon(Icon::Action::Remove).setText("Delete").setEnabled(false).onActivate([&] {
    systemManager.eventRemove();
  });
  quit.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { emulator.quit(); });
}

//

SystemMenu::SystemMenu(MenuBar* parent) : Menu(parent) {
  setText("System");
  power.setText("Power").onToggle([&] { emulator.power(power.checked()); });
  unload.setIcon(Icon::Go::Home).setText("Unload").onActivate([&] {
    emulator.unload();
    programWindow.showPanels();
    programWindow.show(home);
    programWindow.show(systemManager);
    programWindow.setTitle({"higan v", higan::Version});
  });
}

//

SettingsMenu::SettingsMenu(MenuBar* parent) : Menu(parent) {
  setText("Settings");
  outputMenu.setText("Output").setIcon(Icon::Emblem::Image);
  outputCenter.setText("Center").onActivate([&] {
    settings.video.output = "Center";
  });
  outputScale.setText("Scale").onActivate([&] {
    settings.video.output = "Scale";
  });
  outputStretch.setText("Stretch").onActivate([&] {
    settings.video.output = "Stretch";
  });
  if(settings.video.output == "Center") outputCenter.setChecked();
  if(settings.video.output == "Scale") outputScale.setChecked();
  if(settings.video.output == "Stretch") outputStretch.setChecked();
  aspectCorrection.setText("Aspect Correction").setChecked(settings.video.aspectCorrection).onToggle([&] {
    settings.video.aspectCorrection = aspectCorrection.checked();
  });
  scaleMenu.setText("Scale").setIcon(Icon::Emblem::Image);
  scale2x.setText("640x480").onActivate([&] {
    settings.video.scale = 2;
    programWindow.resize();
  });
  scale3x.setText("960x720").onActivate([&] {
    settings.video.scale = 3;
    programWindow.resize();
  });
  scale4x.setText("1280x960").onActivate([&] {
    settings.video.scale = 4;
    programWindow.resize();
  });
  shaderMenu.setText("Shader").setIcon(Icon::Emblem::Image).setEnabled(false);
  video.setText("Video").setIcon(Icon::Device::Display).onActivate([&] {
    programWindow.showPanels();
    programWindow.show(videoSettings);
    systemManager.systemList.selectNone();
    nodeManager.nodeList.selectNone();
  });
  audio.setText("Audio").setIcon(Icon::Device::Speaker).onActivate([&] {
    programWindow.showPanels();
    programWindow.show(audioSettings);
    systemManager.systemList.selectNone();
    nodeManager.nodeList.selectNone();
  });
  input.setText("Input").setIcon(Icon::Device::Joypad).onActivate([&] {
    programWindow.showPanels();
    programWindow.show(inputSettings);
    systemManager.systemList.selectNone();
    nodeManager.nodeList.selectNone();
  });
  hotkeys.setText("Hotkeys").setIcon(Icon::Device::Keyboard).onActivate([&] {
    programWindow.showPanels();
    programWindow.show(hotkeySettings);
    systemManager.systemList.selectNone();
    nodeManager.nodeList.selectNone();
  });
  muteAudio.setText("Mute Audio").setChecked(settings.audio.mute).onToggle([&] {
    settings.audio.mute = muteAudio.checked();
    emulator.audioUpdateEffects();
  });
  showStatusBar.setText("Show Status Bar").setChecked(settings.interface.showStatusBar).onToggle([&] {
    settings.interface.showStatusBar = showStatusBar.checked();
    if(showStatusBar.checked()) {
      programWindow.showStatus();
    } else {
      programWindow.hideStatus();
    }
  });
  showSystemPanels.setText("Show System Panels").setChecked(settings.interface.showSystemPanels).onToggle([&] {
    settings.interface.showSystemPanels = showSystemPanels.checked();
    if(showSystemPanels.checked()) {
      programWindow.showPanels();
    } else {
      programWindow.hidePanels();
    }
  });
  advancedMode.setText("Advanced Mode").setChecked(settings.interface.advancedMode).onToggle([&] {
    settings.interface.advancedMode = advancedMode.checked();
    if(systemManager.visible()) systemManager.refresh();
    if(systemOverview.visible()) systemOverview.refresh();
    if(nodeManager.visible()) nodeManager.refresh();
  });
}

auto SettingsMenu::updateShaders() -> void {
  shaderMenu.reset();
  if(!videoInstance.ready() || !videoInstance.hasShader()) {
    shaderMenu.setEnabled(false);
    return;
  }
  shaderMenu.setEnabled();

  Group shaders;

  MenuRadioItem none{&shaderMenu};
  none.setText("None").onActivate([&] {
    settings.video.shader = "None";
    emulator.videoUpdateShader();
  });
  shaders.append(none);

  MenuRadioItem blur{&shaderMenu};
  blur.setText("Blur").onActivate([&] {
    settings.video.shader = "Blur";
    emulator.videoUpdateShader();
  });
  shaders.append(blur);

  string location{Path::settings, "Shaders/"};

  if(settings.video.driver == "OpenGL 3.2") {
    for(auto shaderName : directory::folders(location, "*.shader")) {
      if(shaders.objectCount() == 2) shaderMenu.append(MenuSeparator());
      MenuRadioItem item{&shaderMenu};
      item.setText(Location::prefix(shaderName)).onActivate([=] {
        settings.video.shader = {location, shaderName};
        emulator.videoUpdateShader();
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

  pauseEmulation.setText("Pause Emulation");
}

//

HelpMenu::HelpMenu(MenuBar* parent) : Menu(parent) {
  setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setAuthor(higan::Author)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setAlignment(programWindow, {0.5f, programWindow.panels.visible() ? 0.32f : 0.5f})
    .show();
  });
}
