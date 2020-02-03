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
  assignButton.setEnabled((bool)inputList.selected());
  clearButton.setEnabled((bool)inputList.selected());
}

auto HotkeySettings::eventClear() -> void {
  if(auto item = inputList.selected()) {
    activeMapping = inputManager.hotkeys[item.offset()];

    for(auto& device : inputManager.devices) {
      if(!device->isKeyboard()) continue;
      uint groupID = 0, inputID = 0;
      for(auto& input : device->group(groupID)) {
        if(input.name() == "Escape") return eventInput(device, groupID, inputID, 0, 1);
        inputID++;
      }
    }
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

  if(device->isKeyboard() && oldValue == 0 && newValue == 1) {
    if(device->group(groupID).input(inputID).name() == "Escape") {
      activeMapping->resetAssignment();
    } else {
      activeMapping->setAssignment(device, groupID, inputID);
    }

    activeMapping.reset();
    settingsWindow.statusBar.setText();
    settingsWindow.layout.setEnabled();
    settingsWindow.setDismissable(true);

    refresh();
  }
}
