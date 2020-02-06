auto Program::stateSave(uint slot) -> bool {
  if(!emulator) return false;

  string location = {Location::notsuffix(emulator->game.location), ".bs", slot};
  if(auto state = emulator->interface->serialize()) {
    if(file::write(location, {state.data(), state.size()})) {
      showMessage({"Saved state to slot ", slot});
      return true;
    }
  }

  showMessage({"Failed to save state to slot ", slot});
  return false;
}

auto Program::stateLoad(uint slot) -> bool {
  if(!emulator) return false;

  string location = {Location::notsuffix(emulator->game.location), ".bs", slot};
  if(auto memory = file::read(location)) {
    serializer state{memory.data(), (uint)memory.size()};
    if(emulator->interface->unserialize(state)) {
      showMessage({"Loaded state from slot ", slot});
      return true;
    }
  }

  showMessage({"Failed to load state from slot ", slot});
  return false;
}
