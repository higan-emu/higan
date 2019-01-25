WonderSwanColorInterface::WonderSwanColorInterface() {
  propertyWonderSwanColor.memory.size(2048);
}

auto WonderSwanColorInterface::information() -> Information {
  Information information;
  information.manufacturer = "Bandai";
  information.name         = "WonderSwan Color";
  information.extension    = "wsc";
  return information;
}

auto WonderSwanColorInterface::load() -> bool {
  return system.load(this, System::Model::WonderSwanColor);
}

auto WonderSwanColorInterface::properties() -> Settings& {
  return propertyWonderSwanColor;
}
