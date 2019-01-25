WonderSwanInterface::WonderSwanInterface() {
  propertyWonderSwan.memory.size(128);
}

auto WonderSwanInterface::information() -> Information {
  Information information;
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan";
  information.extension    = "ws";
  return information;
}

auto WonderSwanInterface::load() -> bool {
  return system.load(this, System::Model::WonderSwan);
}

auto WonderSwanInterface::properties() -> Settings& {
  return propertyWonderSwan;
}
