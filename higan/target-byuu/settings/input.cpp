auto InputSettings::construct() -> void {
  setCollapsible();
  setVisible(false);

  inputList.setBatchable();
  inputList.setHeadered();
  inputList.onChange([&] { eventChange(); });
  inputList.onActivate([&](auto cell) { eventAssign(); });

  reload();

  spacer.setFocusable();
  assignButton.setText("Assign").onActivate([&] { eventAssign(); });
  clearButton.setText("Clear").onActivate([&] { eventClear(); });
}

auto InputSettings::reload() -> void {
  inputList.reset();
  inputList.append(TableViewColumn().setText("Name"));
  inputList.append(TableViewColumn().setText("Mapping").setExpandable());

  for(auto& mapping : virtualPad.mappings) {
    TableViewItem item{&inputList};
    item.append(TableViewCell().setText(mapping->name).setFont(Font().setBold()));
    item.append(TableViewCell());
  }

  refresh();
  eventChange();
}

auto InputSettings::refresh() -> void {
  uint index = 0;
  for(auto& mapping : virtualPad.mappings) {
    inputList.item(index++).cell(1).setIcon(mapping->icon()).setText(mapping->text());
  }
}

auto InputSettings::eventChange() -> void {
  assignButton.setEnabled(inputList.batched().size() == 1);
  clearButton.setEnabled(inputList.batched().size() >= 1);
}

auto InputSettings::eventClear() -> void {
  for(auto& item : inputList.batched()) {
    activeMapping = *virtualPad.mappings[item.offset()];
    shared_pointer<HID::Device> device{new HID::Null};
    eventInput(device, 0, 0, 0, 0);
  }
}

auto InputSettings::eventAssign() -> void {
  if(auto item = inputList.selected()) {
    activeMapping = *virtualPad.mappings[item.offset()];

    settingsWindow.statusBar.setText({"Press a key or button to assign to [", activeMapping->name, "] ..."});
    settingsWindow.layout.setEnabled(false);
    settingsWindow.setDismissable(false);
    spacer.setFocused();
  }
}

auto InputSettings::eventInput(shared_pointer<HID::Device> device, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void {
  if(!activeMapping) return;
  if(!settingsWindow.focused()) return;

  if(activeMapping->bind(device, groupID, inputID, oldValue, newValue)) {
    activeMapping.reset();
    settingsWindow.statusBar.setText();
    settingsWindow.layout.setEnabled();
    settingsWindow.setDismissable(true);

    refresh();
    inputList.setFocused();
  }
}
