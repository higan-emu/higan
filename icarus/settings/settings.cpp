Settings settings;

auto Settings::serialize() -> string {
  string s;
  #define write(name) s.append(#name, ": ", name, "\n")
  write(createManifests);
  write(useDatabase);
  write(useHeuristics);
  write(recent);
  write(bsMemory);
  write(colecoVision);
  write(famicom);
  write(gameBoy);
  write(gameBoyAdvance);
  write(gameBoyColor);
  write(gameGear);
  write(masterSystem);
  write(megaDrive);
  write(msx);
  write(neoGeoPocket);
  write(neoGeoPocketColor);
  write(pcEngine);
  write(pocketChallengeV2);
  write(sc3000);
  write(sg1000);
  write(sufamiTurbo);
  write(superFamicom);
  write(superGrafx);
  write(wonderSwan);
  write(wonderSwanColor);
  #undef write
  return s;
}

auto Settings::unserialize(const string& markup) -> void {
  *this = {};
  auto s = BML::unserialize(markup);
  #define read(name, type) if(auto node = s[#name]) name = node.type()
  read(createManifests, boolean);
  read(useDatabase, boolean);
  read(useHeuristics, boolean);
  read(recent, string);
  read(bsMemory, string);
  read(colecoVision, string);
  read(famicom, string);
  read(gameBoy, string);
  read(gameBoyAdvance, string);
  read(gameBoyColor, string);
  read(gameGear, string);
  read(masterSystem, string);
  read(megaDrive, string);
  read(msx, string);
  read(neoGeoPocket, string);
  read(neoGeoPocketColor, string);
  read(pcEngine, string);
  read(pocketChallengeV2, string);
  read(sc3000, string);
  read(sg1000, string);
  read(sufamiTurbo, string);
  read(superFamicom, string);
  read(superGrafx, string);
  read(wonderSwan, string);
  read(wonderSwanColor, string);
  #undef read
}
