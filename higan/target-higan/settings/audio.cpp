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
  exclusiveOption.setText("Exclusive");
  synchronizeOption.setText("Synchronize");
  dynamicRateOption.setText("Dynamic rate");
  effectsHeader.setText("Effects").setFont(Font().setBold());
  effectsLayout.setSize({3, 3});
  effectsLayout.column(0).setAlignment(1.0);
  effectsLayout.column(1).setAlignment(0.5);
  skewLabel.setText("Skew:");
  volumeLabel.setText("Volume:");
  balanceLabel.setText("Balance:");
}

auto AudioSettings::show() -> void {
  setVisible(true);
}

auto AudioSettings::hide() -> void {
  setVisible(false);
}
