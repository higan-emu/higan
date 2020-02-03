#include "../byuu.hpp"
#include "famicom.cpp"

vector<shared_pointer<Emulator>> emulators;
shared_pointer<Emulator> emulator;

auto Emulator::construct() -> void {
  #ifdef CORE_FC
  emulators.append(new FamicomEmulator);
  #endif
}

auto Emulator::load(const string& name, const vector<uint8_t>& data) -> void {
  settings.gamePath = Location::dir(name);

  game.name = name;
  game.data = data;

  auto system = higan::Node::System::create();
  system->setName(interface->name());
  auto configuration = higan::Node::serialize(system);
  interface->load(root, configuration);

  load();
  interface->power();
}

auto Emulator::unload() -> void {
  interface->unload();
}
