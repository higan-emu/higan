struct Presentation : Window {
  Presentation();
  auto resizeWindow() -> void;
  auto loadEmulators() -> void;
  auto loadEmulator() -> void;
  auto unloadEmulator() -> void;
  auto loadShaders() -> void;

  MenuBar menuBar{this};
    Menu loadMenu{&menuBar};
    Menu systemMenu{&menuBar};
    Menu settingsMenu{&menuBar};
      Menu videoSizeMenu{&settingsMenu};
      Menu videoOutputMenu{&settingsMenu};
        MenuRadioItem videoOutputCenter{&videoOutputMenu};
        MenuRadioItem videoOutputScale{&videoOutputMenu};
        MenuRadioItem videoOutputStretch{&videoOutputMenu};
        Group videoOutputGroup{&videoOutputCenter, &videoOutputScale, &videoOutputStretch};
        MenuSeparator videoOutputSeparator{&videoOutputMenu};
        MenuCheckItem videoAspectCorrection{&videoOutputMenu};
        MenuCheckItem videoAdaptiveSizing{&videoOutputMenu};
        MenuCheckItem videoAutoCentering{&videoOutputMenu};
      Menu videoShaderMenu{&settingsMenu};
      MenuSeparator videoSettingsSeparatpr{&settingsMenu};
      MenuCheckItem muteAudioSetting{&settingsMenu};
      MenuCheckItem showStatusBarSetting{&settingsMenu};
      MenuSeparator audioSettingsSeparator{&settingsMenu};
      MenuItem videoSettingsAction{&settingsMenu};
      MenuItem audioSettingsAction{&settingsMenu};
      MenuItem inputSettingsAction{&settingsMenu};
      MenuItem hotkeySettingsAction{&settingsMenu};
      MenuItem emulatorSettingsAction{&settingsMenu};
      MenuItem driverSettingsAction{&settingsMenu};
    Menu toolsMenu{&menuBar};
      Menu saveStateMenu{&toolsMenu};
      Menu loadStateMenu{&toolsMenu};
      MenuSeparator stateMenuSeparator{&toolsMenu};
      MenuCheckItem pauseEmulation{&toolsMenu};
    Menu helpMenu{&menuBar};
      MenuItem webpageAction{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem aboutAction{&helpMenu};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};

  StatusBar statusBar{this};
};

namespace Instances { extern Instance<Presentation> presentation; }
extern Presentation& presentation;
