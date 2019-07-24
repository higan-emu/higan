HotkeySettings::HotkeySettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);

  headerLabel.setText("Hotkeys").setFont(Font().setBold());
  hotkeyList.setBatchable();
  hotkeyList.onActivate([&] { eventAssign(); });
  hotkeyList.onChange([&] { eventChange(); });
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
}

auto HotkeySettings::update() -> void {
  for(auto& item : hotkeyList.items()) {
    auto value = item.cell(1);
    auto hotkey = item.property<InputHotkey*>("hotkey");
    if(hotkey->device) {
      string label{hotkey->device->name()};
      if(hotkey->device->isJoypad()) label.append(".", hotkey->device->group(hotkey->groupID).name());
      label.append(".", hotkey->device->group(hotkey->groupID).input(hotkey->inputID).name());
      value.setText(label).setFont();
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
    hotkeyList.selectNone().setFocused();
    return eventChange();
  }

  //use the viewport to sink inputs away from the table view during assignment
  programWindow.viewport.setFocused();
  auto item = assigningQueue.takeFirst();
  hotkeyList.selectNone();
  item.setSelected().setFocused();
  auto hotkey = item.property<InputHotkey*>("hotkey");
  item.cell(1).setText("(assign)");
  assigning = *hotkey;
  eventChange();
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
  if(!assigning) return;

  bool allow = false;

  if(device->isKeyboard()) {
    if(oldValue == 0 && newValue == 1) allow = true;
  }
  if(device->isJoypad() && group == HID::Joypad::Button) {
    if(oldValue == 0 && newValue == 1) allow = true;
  }

  if(!allow) return;

  assigning->identifier = {};
  if(device->group(group).input(input).name() != "Escape") {
    assigning->identifier.append(hex(device->pathID()), "/");
    assigning->identifier.append(hex(device->vendorID()), "/");
    assigning->identifier.append(hex(device->productID()), "/");
    assigning->identifier.append(device->group(group).name(), "/");
    assigning->identifier.append(device->group(group).input(input).name());
  }
  assigning.reset();
  hotkeys.bind();
  update();
  eventAssignNext();
}
