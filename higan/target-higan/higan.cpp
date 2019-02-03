#include "higan.hpp"

Video video;
Audio audio;
Input input;
shared_pointer<higan::Interface> emulator;
vector<shared_pointer<higan::Interface>> emulators;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  directory::create({Path::userData(), "higan/"});
  return {Path::userData(), "higan/", name};
}

auto hiro::initialize() -> void {
}

#include <fc/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <ms/interface/interface.hpp>
#include <msx/interface/interface.hpp>
#include <ngp/interface/interface.hpp>
#include <pce/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <ws/interface/interface.hpp>

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("higan");
  Application::setScreenSaver(false);
  #ifdef CORE_FC
  emulators.append(new higan::Famicom::FamicomInterface);
  #endif
  #ifdef CORE_SFC
  emulators.append(new higan::SuperFamicom::SuperFamicomInterface);
  #endif
  #ifdef CORE_MS
  emulators.append(new higan::MasterSystem::SG1000Interface);
  emulators.append(new higan::MasterSystem::SC3000Interface);
  emulators.append(new higan::MasterSystem::MasterSystemInterface);
  #endif
  #ifdef CORE_MD
  emulators.append(new higan::MegaDrive::MegaDriveInterface);
  #endif
  #ifdef CORE_PCE
  emulators.append(new higan::PCEngine::PCEngineInterface);
  emulators.append(new higan::PCEngine::SuperGrafxInterface);
  #endif
  #ifdef CORE_MS
  emulators.append(new higan::MasterSystem::ColecoVisionInterface);
  #endif
  #ifdef CORE_MSX
  emulators.append(new higan::MSX::MSXInterface);
  #endif
  #ifdef CORE_GB
  emulators.append(new higan::GameBoy::GameBoyInterface);
  emulators.append(new higan::GameBoy::GameBoyColorInterface);
  #endif
  #ifdef CORE_GBA
  emulators.append(new higan::GameBoyAdvance::GameBoyAdvanceInterface);
  #endif
  #ifdef CORE_MS
  emulators.append(new higan::MasterSystem::GameGearInterface);
  #endif
  #ifdef CORE_WS
  emulators.append(new higan::WonderSwan::WonderSwanInterface);
  emulators.append(new higan::WonderSwan::WonderSwanColorInterface);
  emulators.append(new higan::WonderSwan::PocketChallengeV2Interface);
  #endif
  #ifdef CORE_NGP
  emulators.append(new higan::NeoGeoPocket::NeoGeoPocketInterface);
  emulators.append(new higan::NeoGeoPocket::NeoGeoPocketColorInterface);
  #endif
  Instances::connectionManager.construct();
  Instances::configurationManager.construct();
  Instances::viewport.construct();
  configurationManager.show();
  Application::run();
}
