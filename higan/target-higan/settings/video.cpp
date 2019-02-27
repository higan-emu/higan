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
  exclusiveOption.setText("Exclusive");
  synchronizeOption.setText("Synchronize");
  flushOption.setText("Flush");
  auto width = max(driverLabel.minimumSize().width(), formatLabel.minimumSize().height());
  driverLayout.cell(0).setSize({width, 0});
  optionsLayout.cell(0).setSize({width, 0});
  colorHeader.setText("Color Adjustment").setFont(Font().setBold());
  colorLayout.setSize({3, 3});
  colorLayout.column(0).setAlignment(1.0);
  colorLayout.column(1).setAlignment(0.5);
  luminanceLabel.setText("Luminance:");
  saturationLabel.setText("Saturation:");
  gammaLabel.setText("Gamma:");
}

auto VideoSettings::show() -> void {
  setVisible(true);
}

auto VideoSettings::hide() -> void {
  setVisible(false);
}
