PocketChallengeV2Interface::PocketChallengeV2Interface() {
  propertyPocketChallengeV2.memory.size(128);
}

auto PocketChallengeV2Interface::information() -> Information {
  Information information;
  information.manufacturer = "Benesse";
  information.name         = "Pocket Challenge V2";
  information.extension    = "pc2";
  return information;
}

auto PocketChallengeV2Interface::load() -> bool {
  return system.load(this, System::Model::PocketChallengeV2);
}

auto PocketChallengeV2Interface::properties() -> Settings& {
  return propertyPocketChallengeV2;
}
