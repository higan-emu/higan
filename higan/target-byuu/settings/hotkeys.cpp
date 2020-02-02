HotkeySettings::HotkeySettings() {
  setCollapsible();
  setVisible(false);

  inputList.setHeadered();
  inputList.onChange([&] { eventChange(); });
  inputList.onActivate([&](auto cell) { eventAssign(); });

  reload();

  assignButton.setText("Assign").onActivate([&] { eventAssign(); });
  clearButton.setText("Clear").onActivate([&] { eventClear(); });
}

auto HotkeySettings::reload() -> void {
  inputList.reset();
  inputList.append(TableViewColumn().setText("Name"));
  inputList.append(TableViewColumn().setText("Mapping").setExpandable());

  refresh();
  eventChange();
}

auto HotkeySettings::refresh() -> void {
}

auto HotkeySettings::eventChange() -> void {
}

auto HotkeySettings::eventClear() -> void {
}

auto HotkeySettings::eventAssign() -> void {
}

auto HotkeySettings::eventInput(shared_pointer<HID::Device> device, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void {
  if(!activeMapping) return;
  if(!settingsWindow.focused()) return;
}
