VideoSettings::VideoSettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  driverHeader.setText("Video Settings").setFont(Font().setBold());
  driverLabel.setText("Driver:").setAlignment(1.0);
  for(auto& driver : Video::hasDrivers()) {
    ComboButtonItem item{&driverOption};
    item.setText(driver);
    if(driver == settings.video.driver) {
      item.setIcon(Icon::Emblem::Program).setSelected();
    } else {
      item.setIcon(Icon::Emblem::Binary);
    }
  }
  driverOption.onChange([&] {
    settings.video.driver = driverOption.selected().text();
  });
  formatLabel.setText("Format:").setAlignment(1.0);
  exclusiveOption.setText("Exclusive").setChecked(settings.video.exclusive).onToggle([&] {
    settings.video.exclusive = exclusiveOption.checked();
  });
  synchronizeOption.setText("Synchronize").setChecked(settings.video.synchronize).onToggle([&] {
    settings.video.synchronize = synchronizeOption.checked();
  });
  flushOption.setText("Flush").setChecked(settings.video.flush).onToggle([&] {
    settings.video.flush = flushOption.checked();
  });
  auto width = max(driverLabel.minimumSize().width(), formatLabel.minimumSize().width());
  driverLayout.cell(0).setSize({width, 0});
  optionsLayout.cell(0).setSize({width, 0});
  colorHeader.setText("Color Adjustment").setFont(Font().setBold());
  colorLayout.setSize({3, 3});
  colorLayout.column(0).setAlignment(1.0);
  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings.video.luminance * 100.0).onChange([&] {
    settings.video.luminance = luminanceSlider.position() / 100.0;
    luminanceValue.setText({luminanceSlider.position(), "%"});
  }).doChange();
  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings.video.saturation * 100.0).onChange([&] {
    settings.video.saturation = saturationSlider.position() / 100.0;
    saturationValue.setText({saturationSlider.position(), "%"});
  }).doChange();
  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition((settings.video.gamma - 1.0) * 100.0).onChange([&] {
    settings.video.gamma = 1.0 + gammaSlider.position() / 100.0;
    gammaValue.setText({100 + gammaSlider.position(), "%"});
  }).doChange();
}

auto VideoSettings::show() -> void {
  setVisible(true);
}

auto VideoSettings::hide() -> void {
  setVisible(false);
}
