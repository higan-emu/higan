struct Settings {
  auto serialize() -> string;
  auto unserialize(const string&) -> void;

  boolean createManifests  = false;
  boolean useDatabase      = true;
  boolean useHeuristics    = true;
  string recent            = {Path::user()};
  string bsMemory          = {Path::user(), "Emulation/BS Memory/"};
  string colecoVision      = {Path::user(), "Emulation/ColecoVision/"};
  string famicom           = {Path::user(), "Emulation/Famicom/"};
  string gameBoy           = {Path::user(), "Emulation/Game Boy/"};
  string gameBoyAdvance    = {Path::user(), "Emulation/Game Boy Advance/"};
  string gameBoyColor      = {Path::user(), "Emulation/Game Boy Color/"};
  string gameGear          = {Path::user(), "Emulation/Game Gear/"};
  string masterSystem      = {Path::user(), "Emulation/Master System/"};
  string megaDrive         = {Path::user(), "Emulation/Mega Drive/"};
  string msx               = {Path::user(), "Emulation/MSX/"};
  string neoGeoPocket      = {Path::user(), "Emulation/Neo Geo Pocket/"};
  string neoGeoPocketColor = {Path::user(), "Emulation/Neo Geo Pocket Color/"};
  string pcEngine          = {Path::user(), "Emulation/PC Engine/"};
  string pocketChallengeV2 = {Path::user(), "Emulation/Pocket Challenge V2/"};
  string sc3000            = {Path::user(), "Emulation/SC-3000/"};
  string sg1000            = {Path::user(), "Emulation/SG-1000/"};
  string sufamiTurbo       = {Path::user(), "Emulation/Sufami Turbo/"};
  string superFamicom      = {Path::user(), "Emulation/Super Famicom/"};
  string superGrafx        = {Path::user(), "Emulation/SuperGrafx/"};
  string wonderSwan        = {Path::user(), "Emulation/WonderSwan/"};
  string wonderSwanColor   = {Path::user(), "Emulation/WonderSwan Color/"};
};

extern Settings settings;
