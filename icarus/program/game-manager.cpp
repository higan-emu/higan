GameManager::GameManager(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  pathLabel.setFont(Font().setBold()).setForegroundColor({0, 0, 240});
  pathLabel.onMouseRelease([&](auto button) {
    if(!path || button != Mouse::Button::Left) return;
    if(auto location = BrowserDialog()
    .setTitle({"Set ", system, " Games Location"})
    .setPath(*path)
    .setAlignment(programWindow)
    .selectFolder()
    ) {
      *path = location;
      pathLabel.setText(*path);
      refresh();
    }
  });
  importButton.setText("Import ...").onActivate([&] {
    if(auto files = BrowserDialog()
    .setTitle({"Import ", system, " Games"})
    .setPath(settings.recent)
    .setAlignment(programWindow)
    .openFiles()
    ) {
      settings.recent = Location::path(files.first());
      gameImporter.import(system, files);
    }
  });
}

auto GameManager::select(string system) -> void {
  path.reset();
  if(system == "BS Memory"           ) path = settings.bsMemory;
  if(system == "ColecoVision"        ) path = settings.colecoVision;
  if(system == "Famicom"             ) path = settings.famicom;
  if(system == "Game Boy"            ) path = settings.gameBoy;
  if(system == "Game Boy Advance"    ) path = settings.gameBoyAdvance;
  if(system == "Game Boy Color"      ) path = settings.gameBoyColor;
  if(system == "Game Gear"           ) path = settings.gameGear;
  if(system == "Master System"       ) path = settings.masterSystem;
  if(system == "Mega Drive"          ) path = settings.megaDrive;
  if(system == "MSX"                 ) path = settings.msx;
  if(system == "Neo Geo Pocket"      ) path = settings.neoGeoPocket;
  if(system == "Neo Geo Pocket Color") path = settings.neoGeoPocketColor;
  if(system == "PC Engine"           ) path = settings.pcEngine;
  if(system == "Pocket Challenge V2" ) path = settings.pocketChallengeV2;
  if(system == "SC-3000"             ) path = settings.sc3000;
  if(system == "SG-1000"             ) path = settings.sg1000;
  if(system == "Sufami Turbo"        ) path = settings.sufamiTurbo;
  if(system == "Super Famicom"       ) path = settings.superFamicom;
  if(system == "SuperGrafx"          ) path = settings.superGrafx;
  if(system == "WonderSwan"          ) path = settings.wonderSwan;
  if(system == "WonderSwan Color"    ) path = settings.wonderSwanColor;
  if(!path) return;
  pathLabel.setText(*path);
  this->system = system;
  refresh();
}

auto GameManager::refresh() -> void {
  gameList.reset();
  if(!path) return;
  for(auto& name : directory::folders(*path)) {
    ListViewItem item{&gameList};
    item.setIcon(Icon::Emblem::Folder);
    item.setText(string{name}.trimRight("/", 1L));
  }

  programWindow.show(*this);
  Application::processEvents();
  gameList.resize();
}
