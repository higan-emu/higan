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
  toggleStatus.onPress = [&] {
    if(programWindow.statusLayout.visible()) {
      programWindow.hideStatus();
    } else {
      programWindow.showStatus();
    }
  };
  hotkeys.append(&toggleStatus);

  togglePanels.onPress = [&] {
    if(programWindow.panels.visible()) {
      programWindow.hidePanels();
    } else {
      programWindow.showPanels();
    }
  };
  hotkeys.append(&togglePanels);

  toggleFullscreen.onPress = [&] {
    emulator.videoToggleFullscreen();
  };
  hotkeys.append(&toggleFullscreen);

  fastForward.onPress = [&] {
    if(!interface) return;
    fastForwardVideoBlocking = videoInstance.blocking();
    fastForwardAudioBlocking = audioInstance.blocking();
    videoInstance.setBlocking(false);
    audioInstance.setBlocking(false);
  };
  fastForward.onRelease = [&] {
    if(!interface) return;
    videoInstance.setBlocking(fastForwardVideoBlocking);
    audioInstance.setBlocking(fastForwardAudioBlocking);
  };
  hotkeys.append(&fastForward);

  saveState.onPress = [&] {
    emulator.saveState(stateSlot);
  };
  hotkeys.append(&saveState);

  loadState.onPress = [&] {
    emulator.loadState(stateSlot);
  };
  hotkeys.append(&loadState);

  incrementStateSlot.onPress = [&] {
    if(stateSlot < 5) stateSlot++;
    else stateSlot = 1;
  };
  hotkeys.append(&incrementStateSlot);

  decrementStateSlot.onPress = [&] {
    if(stateSlot > 1) stateSlot--;
    else stateSlot = 5;
  };
  hotkeys.append(&decrementStateSlot);
}

auto Hotkeys::poll() -> void {
  for(auto& hotkey : hotkeys) hotkey->poll();
}

auto Hotkeys::bind() -> void {
  for(auto& hotkey : hotkeys) {
    hotkey->device.reset();
    auto part = hotkey->identifier.split("/");
    if(part.size() != 5) continue;

    hotkey->pathID = part[0].hex();
    hotkey->vendorID = part[1].hex();
    hotkey->productID = part[2].hex();
    string groupName = part[3];
    string inputName = part[4];

    for(auto& device : inputManager.devices) {
      if(hotkey->pathID != device->pathID()) continue;
      if(hotkey->vendorID != device->vendorID()) continue;
      if(hotkey->productID != device->productID()) continue;
      if(auto groupID = device->find(groupName)) {
        if(auto inputID = device->group(groupID()).find(inputName)) {
          hotkey->device = device;
          hotkey->groupID = groupID();
          hotkey->inputID = inputID();
        }
      }
    }
  }
}
