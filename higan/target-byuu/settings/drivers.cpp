DriverSettings::DriverSettings() {
  setCollapsible();
  setVisible(false);

  videoLabel.setText("Video").setFont(Font().setBold());
  videoDriverLabel.setText("Driver:");
  for(auto& driver : ruby::Video::hasDrivers()) {
    ComboButtonItem item{&videoDriverList};
    item.setText(driver);
    if(driver == ruby::video.driver()) item.setSelected();
  }
  videoDriverUpdate.setText("Reload").onActivate([&] {});

  audioLabel.setText("Audio").setFont(Font().setBold());
  audioDriverLabel.setText("Driver:");
  for(auto& driver : ruby::Audio::hasDrivers()) {
    ComboButtonItem item{&audioDriverList};
    item.setText(driver);
    if(driver == ruby::audio.driver()) item.setSelected();
  }
  audioDriverUpdate.setText("Reload").onActivate([&] {});

  inputLabel.setText("Input").setFont(Font().setBold());
  inputDriverLabel.setText("Driver:");
  for(auto& driver : ruby::Input::hasDrivers()) {
    ComboButtonItem item{&inputDriverList};
    item.setText(driver);
    if(driver == ruby::input.driver()) item.setSelected();
  }
  inputDriverUpdate.setText("Reload").onActivate([&] {});
}
