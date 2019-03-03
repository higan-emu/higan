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
  hotkeyList.append(TableViewColumn().setText("Name"));
  hotkeyList.append(TableViewColumn().setText("Mapping").setExpandable());
  for(auto& hotkey : hotkeys.hotkeys) {
    TableViewItem item{&hotkeyList};
    item.setProperty<InputHotkey*>("hotkey", &hotkey);
    TableViewCell name{&item};
    name.setText(hotkey.name).setFont(Font().setBold());
    TableViewCell value{&item};
    if(hotkey.device) {
      value.setText(hotkey.device->group(hotkey.groupID).input(hotkey.inputID).name());
    } else {
      value.setText("(unmapped)").setFont(Font().setSize(7.0));
    }
  }
  hotkeyList.resizeColumns().doChange();
  message.setText();
}

auto HotkeySettings::eventAssign() -> void {
  auto batched = hotkeyList.batched();
  for(auto& item : batched) {
    auto hotkey = item.property<InputHotkey*>("hotkey");
    message.setText({"Assign: ", hotkey->name});
    hotkeyList.setEnabled(false);
    assignButton.setEnabled(false);
    clearButton.setEnabled(false);
    assigning = *hotkey;
  }
}

auto HotkeySettings::eventClear() -> void {
  if(auto batched = hotkeyList.batched()) {
    for(auto& item : batched) {
      auto hotkey = item.property<InputHotkey*>("hotkey");
      hotkey->device.reset();
      hotkey->pathID = 0;
      hotkey->vendorID = 0;
      hotkey->productID = 0;
      hotkey->groupID = 0;
      hotkey->inputID = 0;
    }
    hotkeys.bind();
    refresh();
  }
}

auto HotkeySettings::eventChange() -> void {
  auto batched = hotkeyList.batched();
  assignButton.setEnabled(batched.size() == 1);
  clearButton.setEnabled((bool)batched);
}

auto HotkeySettings::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(!assigning || !device->isKeyboard()) return;
  assigning->device = device;
  assigning->pathID = device->pathID();
  assigning->vendorID = device->vendorID();
  assigning->productID = device->productID();
  assigning->groupID = group;
  assigning->inputID = input;
  assigning.reset();
  hotkeys.bind();
  refresh();
}
