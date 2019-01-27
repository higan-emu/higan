#include "../higan.hpp"
#include <fc/interface/interface.hpp>
#include <sfc/interface/interface.hpp>
#include <ms/interface/interface.hpp>
#include <md/interface/interface.hpp>
#include <pce/interface/interface.hpp>
#include <msx/interface/interface.hpp>
#include <gb/interface/interface.hpp>
#include <gba/interface/interface.hpp>
#include <ws/interface/interface.hpp>
#include <ngp/interface/interface.hpp>

Program program;
#include "platform.cpp"

auto Program::construct(Arguments arguments) -> void {
  higan::platform = this;
  emulator = new higan::Interface;

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

  Instances::presentation.construct();
  presentation.setVisible();

  Instances::settingsWindow.construct();

  video.create(settings.video.driver());
  video.setContext(presentation.viewport.handle());
  video.setBlocking(settings.video.blocking());

  audio.create(settings.audio.driver());
  audio.setContext(presentation.viewport.handle());
  audio.setBlocking(settings.audio.blocking());
  audio.setFrequency(settings.audio.frequency());

  input.create(settings.input.driver());
  input.setContext(presentation.viewport.handle());

  if(string name; arguments.find("--system", name)) {
    for(auto emulator : emulators) {
      if(emulator->information().name == name) {
        activate(emulator);
        break;
      }
    }
  }

  Application::onMain({&Program::main, this});
}

auto Program::main() -> void {
  if(presentation.powerStatus.checked()) {
    emulator->run();
    return;
  }

  if(!Application::modal()) usleep(20 * 1000);
}

auto Program::quit() -> void {
  deactivate();
  settings.save();
  video.reset();
  audio.reset();
  input.reset();
  Application::kill();
}

auto Program::activate(shared_pointer<higan::Interface> interface) -> void {
  emulator = interface;
  slots.append(Slot());
  slots.last().location = locate({"Systems/", emulator->information().name, ".sys/"});
  presentation.activate();
  emulator->load();
}

auto Program::deactivate() -> void {
  emulator->unload();
  emulator = new higan::Interface;
  presentation.deactivate();
  slots.reset();
}

auto Program::power(bool power) -> void {
  if(power) {
    emulator->power();
  }
  presentation.power(power);
}
