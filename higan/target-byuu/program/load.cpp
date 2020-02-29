auto Program::identify(const string& filename) -> shared_pointer<Emulator> {
  auto suffix = Location::suffix(filename).trimLeft(".", 1L).downcase();

  //ZIP file drag-and-drop support will only work on the first file of the archive with this code
  if(suffix == "zip") {
    Decode::ZIP archive;
    if(!archive.open(filename)) return {};
    if(!archive.file) return {};
    suffix = Location::suffix(archive.file.first().name).trimLeft(".", 1L).downcase();
  }

  for(auto& emulator : emulators) {
    for(auto& extension : emulator->extensions) {
      if(suffix == extension) return emulator;
    }
  }

  return {};
}

auto Program::load(shared_pointer<Emulator> emulator, string filename) -> bool {
  if(!filename) {
    string location = emulator->configuration.game;
    if(!location) location = Path::desktop();

    BrowserDialog dialog;
    dialog.setTitle({"Load ", emulator->name, " Game"});
    dialog.setPath(location);
    dialog.setAlignment(presentation);
    string filters{"*.zip:"};
    for(auto& extension : emulator->extensions) {
      filters.append("*.", extension, ":");
    }
    //support both uppercase and lowercase extensions
    filters.append(string{filters}.upcase());
    filters.trimRight(":", 1L);
    filters.prepend(emulator->name, " Games|");
    dialog.setFilters({filters, "All Files|*"});
    filename = openFile(dialog);
  }
  if(!file::exists(filename)) return false;

  vector<uint8_t> filedata;

  if(filename.iendsWith(".zip")) {
    Decode::ZIP archive;
    if(archive.open(filename)) {
      for(auto& file : archive.file) {
        auto extension = Location::suffix(file.name).trimLeft(".", 1L).downcase();
        if(emulator->extensions.find(extension)) {
          filedata = archive.extract(file);
          break;
        }
      }
    }
  } else {
    filedata = file::read(filename);
  }
  if(!filedata) return false;

  //apply patch (if one exists)
  bool patchApplied = false;
  if(auto patch = file::read({Location::notsuffix(filename), ".bps"})) {
    if(auto output = Beat::Single::apply(filedata, patch)) {
      filedata = output();
      patchApplied = true;
    }
  }

  unload();
  ::emulator = emulator;
  emulator->load(filename, filedata);
  paletteUpdate();
  runAheadUpdate();
  presentation.loadEmulator();
  if(settings.video.adaptiveSizing) presentation.resizeWindow();
  state = {};  //reset hotkey state slot to 1
  pause(false);
  showMessage({"Loaded ", Location::prefix(emulator->game.location), patchApplied ? ", and patch applied" : ""});

  //update recent games list
  for(int index = 7; index >= 0; index--) {
    settings.recent.game[index + 1] = settings.recent.game[index];
  }
  settings.recent.game[0] = filename;
  presentation.loadEmulators();

  return true;
}

auto Program::unload() -> void {
  if(!emulator) return;

  settings.save();
  showMessage({"Unloaded ", Location::prefix(emulator->game.location)});
  emulator->unload();
  emulator.reset();
  rewindReset();
  presentation.unloadEmulator();
  message.text = "";
  ruby::video.clear();
  ruby::audio.clear();
}
