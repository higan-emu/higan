Icarus::Icarus() {
  Database::BSMemory          = BML::unserialize(string::read(locate("Database/BS Memory.bml")));
  Database::ColecoVision      = BML::unserialize(string::read(locate("Database/ColecoVision.bml")));
  Database::Famicom           = BML::unserialize(string::read(locate("Database/Famicom.bml")));
  Database::GameBoy           = BML::unserialize(string::read(locate("Database/Game Boy.bml")));
  Database::GameBoyAdvance    = BML::unserialize(string::read(locate("Database/Game Boy Advance.bml")));
  Database::GameBoyColor      = BML::unserialize(string::read(locate("Database/Game Boy Color.bml")));
  Database::GameGear          = BML::unserialize(string::read(locate("Database/Game Gear.bml")));
  Database::MasterSystem      = BML::unserialize(string::read(locate("Database/Master System.bml")));
  Database::MegaDrive         = BML::unserialize(string::read(locate("Database/Mega Drive.bml")));
  Database::MSX               = BML::unserialize(string::read(locate("Database/MSX.bml")));
  Database::NeoGeoPocket      = BML::unserialize(string::read(locate("Database/Neo Geo Pocket.bml")));
  Database::NeoGeoPocketColor = BML::unserialize(string::read(locate("Database/Neo Geo Pocket Color.bml")));
  Database::PCEngine          = BML::unserialize(string::read(locate("Database/PC Engine.bml")));
  Database::PocketChallengeV2 = BML::unserialize(string::read(locate("Database/Pocket Challenge V2.bml")));
  Database::SC3000            = BML::unserialize(string::read(locate("Database/SC-3000.bml")));
  Database::SG1000            = BML::unserialize(string::read(locate("Database/SG-1000.bml")));
  Database::SufamiTurbo       = BML::unserialize(string::read(locate("Database/Sufami Turbo.bml")));
  Database::SuperFamicom      = BML::unserialize(string::read(locate("Database/Super Famicom.bml")));
  Database::SuperGrafx        = BML::unserialize(string::read(locate("Database/SuperGrafx.bml")));
  Database::WonderSwan        = BML::unserialize(string::read(locate("Database/WonderSwan.bml")));
  Database::WonderSwanColor   = BML::unserialize(string::read(locate("Database/WonderSwan Color.bml")));
}

auto Icarus::error() const -> string {
  return errorMessage;
}

auto Icarus::missing() const -> vector<string> {
  return missingFiles;
}

auto Icarus::success(string location) -> string {
  errorMessage = "";
  return location;
}

auto Icarus::failure(string message) -> string {
  errorMessage = message;
  return {};
}

auto Icarus::manifest(string system, string location) -> string {
  location.transform("\\", "/").trimRight("/").append("/");
  if(!directory::exists(location)) return {};

  if(system == "BS Memory"           ) return bsMemoryManifest(location);
  if(system == "ColecoVision"        ) return colecoVisionManifest(location);
  if(system == "Famicom"             ) return famicomManifest(location);
  if(system == "Game Boy"            ) return gameBoyManifest(location);
  if(system == "Game Boy Advance"    ) return gameBoyAdvanceManifest(location);
  if(system == "Game Boy Color"      ) return gameBoyColorManifest(location);
  if(system == "Game Gear"           ) return gameGearManifest(location);
  if(system == "Master System"       ) return masterSystemManifest(location);
  if(system == "Mega Drive"          ) return megaDriveManifest(location);
  if(system == "MSX"                 ) return msxManifest(location);
  if(system == "Neo Geo Pocket"      ) return neoGeoPocketManifest(location);
  if(system == "Neo Geo Pocket Color") return neoGeoPocketColorManifest(location);
  if(system == "PC Engine"           ) return pcEngineManifest(location);
  if(system == "Pocket Challenge V2" ) return pocketChallengeV2Manifest(location);
  if(system == "SC-3000"             ) return sc3000Manifest(location);
  if(system == "SG-1000"             ) return sg1000Manifest(location);
  if(system == "Sufami Turbo"        ) return sufamiTurboManifest(location);
  if(system == "Super Famicom"       ) return superFamicomManifest(location);
  if(system == "SuperGrafx"          ) return superGrafxManifest(location);
  if(system == "WonderSwan"          ) return wonderSwanManifest(location);
  if(system == "WonderSwan Color"    ) return wonderSwanColorManifest(location);

  return {};
}

auto Icarus::import(string system, string location) -> string {
  errorMessage = {};
  missingFiles = {};

  location.transform("\\", "/").trimRight("/");
  if(!file::exists(location)) return failure("file does not exist");
  if(!file::readable(location)) return failure("file is unreadable");

  auto name = Location::prefix(location);
  auto type = Location::suffix(location).downcase();
  if(!name || !type) return failure("invalid file name");

  auto buffer = file::read(location);
  if(!buffer) return failure("file is empty");

  if(type == ".zip") {
    Decode::ZIP zip;
    if(!zip.open(location)) return failure("ZIP archive is invalid");
    if(!zip.file) return failure("ZIP archive is empty");

    name = Location::prefix(zip.file[0].name);
    type = Location::suffix(zip.file[0].name).downcase();
    buffer = zip.extract(zip.file[0]);
  }

  if(system == "BS Memory"           ) return bsMemoryImport(buffer, location);
  if(system == "ColecoVision"        ) return colecoVisionImport(buffer, location);
  if(system == "Famicom"             ) return famicomImport(buffer, location);
  if(system == "Game Boy"            ) return gameBoyImport(buffer, location);
  if(system == "Game Boy Advance"    ) return gameBoyAdvanceImport(buffer, location);
  if(system == "Game Boy Color"      ) return gameBoyColorImport(buffer, location);
  if(system == "Game Gear"           ) return gameGearImport(buffer, location);
  if(system == "Master System"       ) return masterSystemImport(buffer, location);
  if(system == "Mega Drive"          ) return megaDriveImport(buffer, location);
  if(system == "MSX"                 ) return msxImport(buffer, location);
  if(system == "Neo Geo Pocket"      ) return neoGeoPocketImport(buffer, location);
  if(system == "Neo Geo Pocket Color") return neoGeoPocketColorImport(buffer, location);
  if(system == "PC Engine"           ) return pcEngineImport(buffer, location);
  if(system == "Pocket Challenge V2" ) return pocketChallengeV2Import(buffer, location);
  if(system == "SC-3000"             ) return sc3000Import(buffer, location);
  if(system == "SG-1000"             ) return sg1000Import(buffer, location);
  if(system == "Sufami Turbo"        ) return sufamiTurboImport(buffer, location);
  if(system == "Super Famicom"       ) return superFamicomImport(buffer, location);
  if(system == "SuperGrafx"          ) return superGrafxImport(buffer, location);
  if(system == "WonderSwan"          ) return wonderSwanImport(buffer, location);
  if(system == "WonderSwan Color"    ) return wonderSwanColorImport(buffer, location);

  return failure("unrecognized file extension");
}

auto Icarus::concatenate(vector<uint8_t>& output, string location) -> void {
  if(auto input = file::read(location)) {
    auto size = output.size();
    output.resize(size + input.size());
    memory::copy(output.data() + size, input.data(), input.size());
  }
}
