InputSettings::InputSettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  driverHeader.setText("Input Settings").setFont(Font().setBold());
  driverLabel.setText("Driver:").setAlignment(1.0);
  for(auto& driver : Input::hasDrivers()) {
    ComboButtonItem item{&driverOption};
    item.setText(driver);
    if(driver == settings.input.driver) {
      item.setIcon(Icon::Emblem::Program).setSelected();
    } else {
      item.setIcon(Icon::Emblem::Binary);
    }
  }
  driverOption.onChange([&] {
    settings.input.driver = driverOption.selected().text();
  });
  optionsHeader.setText("Options").setFont(Font().setBold());
  focusLabel.setText("When focus is lost:");
  focusPause.setText("Pause emulation");
  focusBlock.setText("Block input");
  focusAllow.setText("Allow input");
}

auto InputSettings::show() -> void {
  setVisible(true);
}

auto InputSettings::hide() -> void {
  setVisible(false);
}
