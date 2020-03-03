#include "../byuu.hpp"
#include <icarus/icarus.hpp>

#ifdef CORE_FC
  #include "famicom.cpp"
#endif

#ifdef CORE_GB
  #include "game-boy.cpp"
#endif

#ifdef CORE_GBA
  #include "game-boy-advance.cpp"
#endif

#ifdef CORE_MD
  #include "mega-drive.cpp"
#endif

#ifdef CORE_MS
  #include "master-system.cpp"
#endif

#ifdef CORE_MSX
  #include "msx.cpp"
#endif

#ifdef CORE_NGP
  #include "neo-geo-pocket.cpp"
#endif

#ifdef CORE_PCE
  #include "pc-engine.cpp"
#endif

#ifdef CORE_SFC
  #include "super-famicom.cpp"
#endif

#ifdef CORE_SG
  #include "sg-1000.cpp"
#endif

#ifdef CORE_WS
  #include "wonderswan.cpp"
#endif

vector<shared_pointer<Emulator>> emulators;
shared_pointer<Emulator> emulator;

auto Emulator::construct() -> void {
  icarus::construct();

  #ifdef CORE_FC
  emulators.append(new Famicom);
  emulators.append(new FamicomDiskSystem);
  #endif

  #ifdef CORE_SFC
  emulators.append(new SuperFamicom);
  #endif

  #ifdef CORE_SG
  emulators.append(new SG1000);
  #endif

  #ifdef CORE_MS
  emulators.append(new MasterSystem);
  #endif

  #ifdef CORE_MD
  emulators.append(new MegaDrive);
  emulators.append(new MegaCD);
  #endif

  #ifdef CORE_PCE
  emulators.append(new PCEngine);
  emulators.append(new SuperGrafx);
  #endif

  #ifdef CORE_MSX
  emulators.append(new MSX);
  emulators.append(new MSX2);
  #endif

  #ifdef CORE_GB
  emulators.append(new GameBoy);
  emulators.append(new GameBoyColor);
  #endif

  #ifdef CORE_GBA
  emulators.append(new GameBoyAdvance);
  #endif

  #ifdef CORE_MS
  emulators.append(new GameGear);
  #endif

  #ifdef CORE_WS
  emulators.append(new WonderSwan);
  emulators.append(new WonderSwanColor);
  emulators.append(new PocketChallengeV2);
  #endif

  #ifdef CORE_NGP
  emulators.append(new NeoGeoPocket);
  emulators.append(new NeoGeoPocketColor);
  #endif
}

auto Emulator::manifest() -> shared_pointer<vfs::file> {
  for(auto& media : icarus::media) {
    if(media->name() != interface->name()) continue;
    if(auto cartridge = media.cast<icarus::Cartridge>()) {
      game.manifest = cartridge->manifest(game.image, game.location);
      return vfs::memory::file::open(game.manifest.data<uint8_t>(), game.manifest.size());
    }
  }
  return {};
}

auto Emulator::load(const string& location, const vector<uint8_t>& image) -> bool {
  configuration.game = Location::dir(location);

  game.location = location;
  game.image = image;

  latch = {};

  auto system = higan::Node::System::create();
  system->setName(interface->name());
  auto configuration = higan::Node::serialize(system);
  interface->load(root, configuration);

  setBoolean("Color Bleed", settings.video.colorBleed);
  setBoolean("Color Emulation", settings.video.colorEmulation);
  setBoolean("Interframe Blending", settings.video.interframeBlending);
  setOverscan(settings.video.overscan);
  if(!load()) return false;

  interface->power();
  return true;
}

auto Emulator::save() -> void {
  interface->save();
}

auto Emulator::unload() -> void {
  interface->unload();
}

auto Emulator::setBoolean(const string& name, bool value) -> bool {
  if(auto node = root->scan<higan::Node::Boolean>(name)) {
    node->setValue(value);
    return true;
  }
  return false;
}

auto Emulator::setOverscan(bool value) -> bool {
  if(auto screen = root->scan<higan::Node::Screen>("Screen")) {
    if(auto region = screen->find<higan::Node::String>("Region")) {
      region->setValue(!settings.video.overscan ? "NTSC" : "PAL");
      return true;
    }
  }
  return false;
}

auto Emulator::error(const string& text) -> void {
  MessageDialog().setTitle("Error").setText(text).setAlignment(presentation).error();
}

auto Emulator::errorFirmwareRequired(const Firmware& firmware) -> void {
  if(MessageDialog().setText({
    emulator->name, " - ", firmware.type, " (", firmware.region, ") is required to play this game.\n"
    "Would you like to configure firmware settings now?"
  }).question() == "Yes") settingsWindow.show("Firmware");
}
