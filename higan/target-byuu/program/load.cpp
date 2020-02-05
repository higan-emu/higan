auto Program::loadGame(shared_pointer<Emulator> emulator) -> void {
  string location = emulator->settings.gamePath;
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
  if(auto filename = dialog.openFile()) {
    vector<uint8_t> filedata;

    if(filename.iendsWith(".zip")) {
      Decode::ZIP archive;
      if(archive.open(filename)) {
        for(auto& file : archive.file) {
          auto extension = Location::suffix(file.name).trimLeft(".").downcase();
          if(emulator->extensions.find(extension)) {
            filedata = archive.extract(file);
            break;
          }
        }
      }
    } else {
      filedata = file::read(filename);
    }

    if(filedata) {
      unloadGame();
      ::emulator = emulator;
      emulator->load(filename, filedata);
      paletteUpdate();
      presentation.loadEmulator();
      state = {};  //reset hotkey state slot to 1
      pause(false);
      showMessage({"Loaded ", Location::prefix(emulator->game.name)});
    }
  }
}

auto Program::unloadGame() -> void {
  if(!emulator) return;

  showMessage({"Unloaded ", Location::prefix(emulator->game.name)});
  emulator->unload();
  emulator.reset();
  presentation.unloadEmulator();
  message.text = "";
  ruby::video.clear();
  ruby::audio.clear();
}
