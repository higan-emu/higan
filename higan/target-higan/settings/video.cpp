VideoSettings::VideoSettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);

  driverHeader.setText("Video Driver").setFont(Font().setBold());
  driverLabel.setText("Driver:");
  for(auto& driver : Video::hasDrivers()) {
    ComboButtonItem item{&driverOption};
    item.setText(driver);
    if(driver == settings.video.driver) item.setSelected();
  }
  activateButton.setText("Activate").onActivate([&] { eventActivate(); });

  settingsHeader.setText("Driver Settings (activate driver to configure)").setFont(Font().setBold());
  settingsLayout.setEnabled(false);
  formatLabel.setText("Format:");
  formatOption.append(ComboButtonItem().setText(settings.video.format)).onChange([&] {
    settings.video.format = formatOption.selected().text();
    refresh();
  });
  exclusiveOption.setText("Exclusive").setChecked(settings.video.exclusive).onToggle([&] {
    settings.video.exclusive = exclusiveOption.checked();
  //handled by fullscreen codepath
  });
  blockingOption.setText("Blocking").setChecked(settings.video.blocking).onToggle([&] {
    settings.video.blocking = blockingOption.checked();
    refresh();
  });
  flushOption.setText("Flush").setChecked(settings.video.flush).onToggle([&] {
    settings.video.flush = flushOption.checked();
    refresh();
  });

  colorHeader.setText("Color Adjustment").setFont(Font().setBold());
  colorLayout.setSize({3, 3});
  colorLayout.column(0).setAlignment(1.0);
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings.video.luminance * 100.0).onChange([&] {
    settings.video.luminance = luminanceSlider.position() / 100.0;
    luminanceValue.setText({luminanceSlider.position(), "%"});
    emulator.videoUpdateColors();
  }).doChange();
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings.video.saturation * 100.0).onChange([&] {
    settings.video.saturation = saturationSlider.position() / 100.0;
    saturationValue.setText({saturationSlider.position(), "%"});
    emulator.videoUpdateColors();
  }).doChange();
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition((settings.video.gamma - 1.0) * 100.0).onChange([&] {
    settings.video.gamma = 1.0 + gammaSlider.position() / 100.0;
    gammaValue.setText({100 + gammaSlider.position(), "%"});
    emulator.videoUpdateColors();
  }).doChange();
  viewport.setVisible(false);
}

auto VideoSettings::show() -> void {
  setVisible(true);
}

auto VideoSettings::hide() -> void {
  setVisible(false);
}

auto VideoSettings::refresh() -> void {
  Video* video = nullptr;
  if(emulator.viewports.size() && emulator.viewports.first()->visible()) {
    video = &emulator.viewports.first()->video;
    emulator.videoUpdate();
  } else {
    viewport.setVisible();
    Application::processEvents();
    video = new Video;
    emulator.videoUpdate(*video, viewport.handle());
  }

  formatOption.reset();
  for(auto& format : video->hasFormats()) {
    ComboButtonItem item{&formatOption};
    item.setText(format);
    if(format == settings.video.format) item.setSelected();
  }
  formatOption.setEnabled(formatOption.itemCount() > 1);

  optionsLayout.resize();

  exclusiveOption.setEnabled(video->hasExclusive());
  blockingOption.setEnabled(video->hasBlocking());
  flushOption.setEnabled(video->hasFlush());

  if(viewport.visible()) {
    video->reset();
    delete video;
    viewport.setVisible(false);
  }
}

auto VideoSettings::eventActivate() -> void {
  activateButton.setText("Change");
  settings.video.driver = driverOption.selected().text();
  refresh();
  settingsHeader.setText({"Driver Settings (", settings.video.driver, ")"});
  settingsLayout.setEnabled(true);
}
