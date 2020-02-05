#include "../byuu.hpp"
#include <icarus/icarus.hpp>

#ifdef CORE_FC
  #include "famicom.cpp"
#endif

#ifdef CORE_PCE
  #include "pc-engine.cpp"
#endif

vector<shared_pointer<Emulator>> emulators;
shared_pointer<Emulator> emulator;

auto Emulator::construct() -> void {
  icarus::construct();

  #ifdef CORE_FC
  emulators.append(new Famicom);
  #endif

  #ifdef CORE_PCE
  emulators.append(new PCEngine);
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
