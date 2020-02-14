auto HotkeySettings::construct() -> void {
  setCollapsible();
  setVisible(false);

  inputList.setBatchable();
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

  for(auto& mapping : inputManager.hotkeys) {
    TableViewItem item{&inputList};
    item.append(TableViewCell().setText(mapping.name).setFont(Font().setBold()));
    item.append(TableViewCell());
  }

  refresh();
  eventChange();
}

auto HotkeySettings::refresh() -> void {
  uint index = 0;
  for(auto& mapping : inputManager.hotkeys) {
    inputList.item(index++).cell(1).setIcon(mapping.icon()).setText(mapping.text());
  }
}

auto HotkeySettings::eventChange() -> void {
  assignButton.setEnabled(inputList.batched().size() == 1);
  clearButton.setEnabled(inputList.batched().size() >= 1);
}

auto HotkeySettings::eventClear() -> void {
  for(auto& item : inputList.batched()) {
    activeMapping = inputManager.hotkeys[item.offset()];
    shared_pointer<HID::Device> device{new HID::Null};
    eventInput(device, 0, 0, 0, 0);
  }
}

auto HotkeySettings::eventAssign() -> void {
  if(auto item = inputList.selected()) {
    activeMapping = inputManager.hotkeys[item.offset()];

    settingsWindow.statusBar.setText({"Press a key or button to assign to [", activeMapping->name, "] ..."});
    settingsWindow.layout.setEnabled(false);
    settingsWindow.setDismissable(false);
  }
}

auto HotkeySettings::eventInput(shared_pointer<HID::Device> device, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void {
  if(!activeMapping) return;
  if(!settingsWindow.focused()) return;

  if(activeMapping->bind(device, groupID, inputID, oldValue, newValue)) {
    activeMapping.reset();
    settingsWindow.statusBar.setText();
    settingsWindow.layout.setEnabled();
    settingsWindow.setDismissable(true);

    refresh();
  }
}
