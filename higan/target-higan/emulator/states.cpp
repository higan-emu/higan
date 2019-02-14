auto Emulator::saveState(uint slot) -> bool {
  if(!system.power) return false;
  if(auto cartridge = root->find<higan::Node::Peripheral>(0)) {
    if(cartridge->type == "Cartridge") {
      if(auto location = cartridge->property("location")) {
        if(auto state = interface->serialize()) {
          directory::create({location, "State/"});
          if(file::write({location, "State/Slot ", slot, ".bst"}, {state.data(), state.size()})) {
            return true;
          }
        }
      }
    }
  }
  return true;
}

auto Emulator::loadState(uint slot) -> bool {
  if(!system.power) return false;
  if(auto cartridge = root->find<higan::Node::Peripheral>(0)) {
    if(cartridge->type == "Cartridge") {
      if(auto location = cartridge->property("location")) {
        if(auto memory = file::read({location, "State/Slot ", slot, ".bst"})) {
          serializer state{memory.data(), (uint)memory.size()};
          if(interface->unserialize(state)) {
            return true;
          }
        }
      }
    }
  }
  return true;
}
