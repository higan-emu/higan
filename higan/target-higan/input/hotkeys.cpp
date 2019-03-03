InputHotkey::InputHotkey(string_view name) : name(name) {
}

auto InputHotkey::poll() -> void {
  if(device) {
    newValue = device->group(groupID).input(inputID).value();
    if(oldValue == 0 && newValue == 1 && onPress) onPress();
    if(oldValue == 1 && newValue == 0 && onRelease) onRelease();
    oldValue = newValue;
  }
}

Hotkeys::Hotkeys() {
  { InputHotkey hotkey{"Save State"};
    hotkey.onPress = [&] { emulator.saveState(stateSlot); };
    hotkeys.append(hotkey);
  }

  { InputHotkey hotkey{"Load State"};
    hotkey.onPress = [&] { emulator.loadState(stateSlot); };
    hotkeys.append(hotkey);
  }

  { InputHotkey hotkey{"Increment State Slot"};
    hotkey.onPress = [&] {
      if(stateSlot < 5) stateSlot++;
      else stateSlot = 1;
    };
    hotkeys.append(hotkey);
  }

  { InputHotkey hotkey{"Decrement State Slot"};
    hotkey.onPress = [&] {
      if(stateSlot > 1) stateSlot--;
      else stateSlot = 5;
    };
    hotkeys.append(hotkey);
  }
}

auto Hotkeys::poll() -> void {
  for(auto& hotkey : hotkeys) hotkey.poll();
}

auto Hotkeys::bind() -> void {
  for(auto& hotkey : hotkeys) {
    hotkey.device.reset();
    for(auto& device : inputManager.devices) {
      if(hotkey.pathID != device->pathID()) continue;
      if(hotkey.vendorID != device->vendorID()) continue;
      if(hotkey.productID != device->productID()) continue;
      hotkey.device = device;
    }
  }
}
