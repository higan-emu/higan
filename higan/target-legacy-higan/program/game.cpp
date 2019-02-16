auto Program::load() -> void {
  if(!gameQueue) return;

  string location = gameQueue.left();
  string extension = Location::suffix(location).trimLeft(".", 1L);

  for(auto& emulator : emulators) {
    auto information = emulator->information();
    if(information.extension == extension) return load(*emulator);
  }

  gameQueue.reset();
}

auto Program::load(higan::Interface& interface) -> void {
  unload();

  auto information = interface.information();
  gamePaths.append(locate({"systems/", information.name, ".sys/"}));

  inputManager->bind(emulator = &interface);

  if(auto properties = string::read({gamePaths[0], "properties.bml"})) {
    emulator->properties().unserialize(properties);
  }
  if(auto properties = emulator->properties().serialize()) {
    file::write({gamePaths[0], "properties.bml"}, properties);
  }

  if(auto options = string::read({gamePaths[0], "options.bml"})) {
    emulator->options().unserialize(options);
  }
  if(auto options = emulator->options().serialize()) {
    file::write({gamePaths[0], "options.bml"}, options);
  }

  presentation->updateEmulatorMenu();
  if(!emulator->load()) {
    emulator = nullptr;
    gamePaths.reset();
    return;
  }
  emulator->power();
  emulator->setOption("video/blurEmulation", presentation->blurEmulation.checked());
  emulator->setOption("video/colorEmulation", presentation->colorEmulation.checked());
  emulator->setOption("video/scanlineEmulation", presentation->scanlineEmulation.checked());
  updateAudioDriver();
  updateAudioEffects();

  presentation->viewportLayout.remove(presentation->iconLayout);
  if(settings["View/Adaptive"].boolean()) {
    presentation->resizeWindow();
  } else {
    presentation->resizeViewport();
  }
  presentation->setTitle(emulator->titles().merge(" + "));
  presentation->systemMenu.setText(information.name).setVisible(true);
  presentation->toolsMenu.setVisible(true);
  toolsManager->cheatEditor.loadCheats();
  toolsManager->stateManager.doRefresh();
  toolsManager->manifestViewer.doRefresh();
  toolsManager->gameNotes.loadNotes();
}

auto Program::unload() -> void {
  if(!emulator) return;

  presentation->clearViewport();
  toolsManager->cheatEditor.saveCheats();
  toolsManager->gameNotes.saveNotes();
  emulator->unload();
  emulator = nullptr;
  gamePaths.reset();

  presentation->viewportLayout.append(presentation->iconLayout, Size{0, ~0});
  if(settings["View/Adaptive"].boolean()) {
    presentation->resizeWindow();
  } else {
    presentation->resizeViewport();
  }
  presentation->setTitle({"higan v", higan::Version});
  presentation->systemMenu.setVisible(false);
  presentation->toolsMenu.setVisible(false);
  toolsManager->setVisible(false);
}
