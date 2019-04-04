auto Icarus::famicomDiskManifest(string location) -> string {
  vector<uint8_t> buffer;
  concatenate(buffer, {location, "program.disk"});
  return famicomDiskManifest(buffer, location);
}

auto Icarus::famicomDiskManifest(vector<uint8_t>& buffer, string location) -> string {
  if(settings.useDatabase) {
    auto digest = Hash::SHA256(buffer).digest();
    for(auto game : Database::FamicomDisk.find("game")) {
      if(game["sha256"].text() == digest) return BML::serialize(game);
    }
  }

  if(settings.useHeuristics) {
    Heuristics::FamicomDisk game{buffer, location};
    if(auto manifest = game.manifest()) return manifest;
  }

  return {};
}

auto Icarus::famicomDiskImport(vector<uint8_t>& buffer, string location) -> string {
  auto name = Location::prefix(location);
  auto source = Location::path(location);
  string target{settings.famicomDisk, name, "/"};

  auto manifest = famicomDiskManifest(buffer, location);
  if(!manifest) return failure("failed to parse ROM image");

  if(!create(target)) return failure("library path unwritable");

  if(settings.createManifests) write({target, "manifest.bml"}, manifest);
  write({target, "program.disk"}, buffer);
  return success(target);
}
