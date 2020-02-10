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
    string location = emulator->path.game;
    if(!location) location = Path::user();

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
    filename = dialog.openFile();
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

  unload();
  ::emulator = emulator;
  emulator->load(filename, filedata);
  paletteUpdate();
  presentation.loadEmulator();
  if(settings.video.adaptiveSizing) presentation.resizeWindow();
  state = {};  //reset hotkey state slot to 1
  pause(false);
  showMessage({"Loaded ", Location::prefix(emulator->game.location)});
  return true;
}

auto Program::unload() -> void {
  if(!emulator) return;

  showMessage({"Unloaded ", Location::prefix(emulator->game.location)});
  emulator->unload();
  emulator.reset();
  presentation.unloadEmulator();
  message.text = "";
  ruby::video.clear();
  ruby::audio.clear();
}
