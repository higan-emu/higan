#include "../bsnes.hpp"
#include "platform.cpp"
#include "game.cpp"
#include "game-pak.cpp"
#include "game-rom.cpp"
#include "paths.cpp"
#include "states.cpp"
#include "video.cpp"
#include "audio.cpp"
#include "input.cpp"
#include "utility.cpp"
#include "patch.cpp"
#include "hacks.cpp"
Instance<Program> programInstance;
Program& program = programInstance();

auto Program::create() -> void {
  higan::platform = this;

  presentationInstance.construct();
  presentation.setVisible();

  videoSettingsInstance.construct();
  audioSettingsInstance.construct();
  inputSettingsInstance.construct();
  hotkeySettingsInstance.construct();
  pathSettingsInstance.construct();
  emulatorSettingsInstance.construct();
  driverSettingsInstance.construct();
  settingsWindowInstance.construct();

  cheatDatabaseInstance.construct();
  cheatWindowInstance.construct();
  cheatEditorInstance.construct();
  stateWindowInstance.construct();
  stateManagerInstance.construct();
  manifestViewerInstance.construct();
  toolsWindowInstance.construct();

  if(settings.general.crashed) {
    MessageDialog(
      "Driver crash detected. Hardware drivers have been disabled.\n"
      "Please reconfigure drivers in the advanced settings panel."
    ).setParent(*presentation).information();
    settings.video.driver = "None";
    settings.audio.driver = "None";
    settings.input.driver = "None";
  }

  settings.general.crashed = true;
  settings.save();
  updateVideoDriver(presentation);
  updateAudioDriver(presentation);
  updateInputDriver(presentation);
  settings.general.crashed = false;
  settings.save();

  driverSettings.videoDriverChanged();
  driverSettings.audioDriverChanged();
  driverSettings.inputDriverChanged();

  if(gameQueue) load();
  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  updateStatus();
  video.poll();
  inputManager.poll();
  inputManager.pollHotkeys();

  if(inactive()) {
    audio.clear();
    if(!Application::modal()) usleep(20 * 1000);
    return;
  }

  emulator->run();
  if(emulatorSettings.autoSaveMemory.checked()) {
    auto currentTime = chrono::timestamp();
    if(currentTime - autoSaveTime >= settings.emulator.autoSaveMemory.interval) {
      autoSaveTime = currentTime;
      emulator->save();
    }
  }
}

auto Program::quit() -> void {
  unload();
  settings.save();
  video.reset();
  audio.reset();
  input.reset();
  Application::kill();
}
