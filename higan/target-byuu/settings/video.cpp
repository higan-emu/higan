VideoSettings::VideoSettings() {
  setCollapsible();
  setVisible(false);

  colorAdjustmentLabel.setText("Color Adjustment").setFont(Font().setBold());
  colorAdjustmentLayout.setSize({3, 3});
  colorAdjustmentLayout.column(0).setAlignment(1.0);

  luminanceLabel.setText("Luminance:");
  luminanceValue.setAlignment(0.5);
  luminanceSlider.setLength(101).setPosition(settings.video.luminance * 100.0).onChange([&] {
    settings.video.luminance = luminanceSlider.position() / 100.0;
    luminanceValue.setText({luminanceSlider.position(), "%"});
    program.paletteUpdate();
  }).doChange();

  saturationLabel.setText("Saturation:");
  saturationValue.setAlignment(0.5);
  saturationSlider.setLength(201).setPosition(settings.video.saturation * 100.0).onChange([&] {
    settings.video.saturation = saturationSlider.position() / 100.0;
    saturationValue.setText({saturationSlider.position(), "%"});
    program.paletteUpdate();
  }).doChange();

  gammaLabel.setText("Gamma:");
  gammaValue.setAlignment(0.5);
  gammaSlider.setLength(101).setPosition((settings.video.gamma - 1.0) * 100.0).onChange([&] {
    settings.video.gamma = 1.0 + gammaSlider.position() / 100.0;
    gammaValue.setText({100 + gammaSlider.position(), "%"});
    program.paletteUpdate();
  }).doChange();
}
