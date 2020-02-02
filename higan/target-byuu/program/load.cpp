auto Program::loadGame(shared_pointer<Emulator> emulator) -> void {
  string location = emulator->settings.gamePath;
  if(!location) location = Path::user();

  BrowserDialog dialog;
  dialog.setTitle({"Load ", emulator->name, " Game"});
  dialog.setPath(location);
  dialog.setAlignment(presentation);
  string filters;  //{"*.zip:"};  //not supported by icarus --manifest yet ...
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
      presentation.loadEmulator();
    }
  }
}

auto Program::unloadGame() -> void {
  if(!emulator) return;

  emulator.reset();
  presentation.unloadEmulator();
  presentation.statusBar.setText();
  ruby::video.clear();
  ruby::audio.clear();
}
