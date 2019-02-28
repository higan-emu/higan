AudioSettings::AudioSettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  driverHeader.setText("Audio Settings").setFont(Font().setBold());
  driverLabel.setText("Driver:").setAlignment(1.0);
  for(auto& driver : Audio::hasDrivers()) {
    ComboButtonItem item{&driverOption};
    item.setText(driver);
    if(driver == settings.audio.driver) {
      item.setIcon(Icon::Emblem::Program).setSelected();
    } else {
      item.setIcon(Icon::Emblem::Binary);
    }
  }
  driverOption.onChange([&] {
    settings.audio.driver = driverOption.selected().text();
  });
  deviceLabel.setText("Device:").setAlignment(1.0);
  frequencyLabel.setText("Frequency:");
  latencyLabel.setText("Latency:");
  auto width = max(driverLabel.minimumSize().width(), deviceLabel.minimumSize().width());
  driverLayout.cell(0).setSize({width, 0});
  optionsLayout.cell(0).setSize({width, 0});
  exclusiveOption.setText("Exclusive").setChecked(settings.audio.exclusive).onToggle([&] {
    settings.audio.exclusive = exclusiveOption.checked();
  });
  synchronizeOption.setText("Synchronize").setChecked(settings.audio.synchronize).onToggle([&] {
    settings.audio.synchronize = synchronizeOption.checked();
  });
  dynamicRateOption.setText("Dynamic rate").setChecked(settings.audio.dynamicRate).onToggle([&] {
    settings.audio.dynamicRate = dynamicRateOption.checked();
  });
  effectsHeader.setText("Effects").setFont(Font().setBold());
  effectsLayout.setSize({3, 3});
  effectsLayout.column(0).setAlignment(1.0);
  skewLabel.setText("Skew:");
  skewValue.setAlignment(0.5);
  skewSlider.setLength(10001).setPosition(settings.audio.skew + 5000).onChange([&] {
    settings.audio.skew = (int)skewSlider.position() - 5000;
    skewValue.setText({settings.audio.skew > 0 ? "+" : "", settings.audio.skew});
  }).doChange();
  volumeLabel.setText("Volume:");
  volumeValue.setAlignment(0.5);
  volumeSlider.setLength(201).setPosition(settings.audio.volume * 100.0).onChange([&] {
    settings.audio.volume = volumeSlider.position() / 100.0;
    volumeValue.setText({volumeSlider.position(), "%"});
  }).doChange();
  balanceLabel.setText("Balance:");
  balanceValue.setAlignment(0.5);
  balanceSlider.setLength(101).setPosition(settings.audio.balance * 100.0).onChange([&] {
    settings.audio.balance = balanceSlider.position() / 100.0;
    balanceValue.setText({balanceSlider.position(), "%"});
  }).doChange();
}

auto AudioSettings::show() -> void {
  setVisible(true);
}

auto AudioSettings::hide() -> void {
  setVisible(false);
}
