HotkeySettings::HotkeySettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);

  headerLabel.setText("Hotkeys").setFont(Font().setBold());
  hotkeyList.setBatchable();
  hotkeyList.onActivate([&] { eventAssign(); });
  hotkeyList.onChange([&] { eventChange(); });
  message.setFont(Font().setBold());
  assignButton.setText("Assign").onActivate([&] { eventAssign(); });
  clearButton.setText("Clear").onActivate([&] { eventClear(); });
}

auto HotkeySettings::show() -> void {
  refresh();
  setVisible(true);
}

auto HotkeySettings::hide() -> void {
  setVisible(false);
  assigning.reset();
}

auto HotkeySettings::refresh() -> void {
  hotkeyList.reset().setEnabled();
  hotkeyList.append(TableViewColumn().setText("Name").setBackgroundColor({240, 240, 255}));
  hotkeyList.append(TableViewColumn().setText("Mapping").setExpandable());
  for(auto& hotkey : hotkeys.hotkeys) {
    TableViewItem item{&hotkeyList};
    item.setProperty<InputHotkey*>("hotkey", hotkey);
    TableViewCell name{&item};
    name.setText(hotkey->name).setFont(Font().setBold());
    TableViewCell value{&item};
  }
  update();
  hotkeyList.doChange();
  message.setText();
}

auto HotkeySettings::update() -> void {
  for(auto& item : hotkeyList.items()) {
    auto value = item.cell(1);
    auto hotkey = item.property<InputHotkey*>("hotkey");
    if(hotkey->device) {
      value.setText(hotkey->device->group(hotkey->groupID).input(hotkey->inputID).name()).setFont();
    } else {
      value.setText("(unmapped)").setFont(Font().setSize(7.0));
    }
  }

  hotkeyList.resizeColumns();
}

auto HotkeySettings::eventAssign() -> void {
  inputManager.poll();
  assigningQueue = hotkeyList.batched();
  eventAssignNext();
}

auto HotkeySettings::eventAssignNext() -> void {
  if(!assigningQueue) {
    hotkeyList.setFocused();
    for(auto& item : hotkeyList.batched()) item.setSelected(false);
    message.setText();
    return;
  }

  programWindow.viewport.setFocused();
  auto item = assigningQueue.takeFirst();
  item.setFocused();
  auto hotkey = item.property<InputHotkey*>("hotkey");
  message.setText({"Assign: ", hotkey->name});
  assignButton.setEnabled(false);
  clearButton.setEnabled(false);
  assigning = *hotkey;
}

auto HotkeySettings::eventClear() -> void {
  if(auto batched = hotkeyList.batched()) {
    for(auto& item : batched) {
      auto hotkey = item.property<InputHotkey*>("hotkey");
      hotkey->identifier = {};
    }
    hotkeys.bind();
    update();
  }
}

auto HotkeySettings::eventChange() -> void {
  auto batched = hotkeyList.batched();
  assignButton.setEnabled((bool)batched);
  clearButton.setEnabled((bool)batched);
}

auto HotkeySettings::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(!assigning || !device->isKeyboard() || oldValue == 1 || newValue == 0) return;
  assigning->identifier = {};
  assigning->identifier.append(hex(device->pathID()), "/");
  assigning->identifier.append(hex(device->vendorID()), "/");
  assigning->identifier.append(hex(device->productID()), "/");
  assigning->identifier.append(device->group(group).name(), "/");
  assigning->identifier.append(device->group(group).input(input).name());
  assigning.reset();
  hotkeys.bind();
  update();
  eventAssignNext();
}
