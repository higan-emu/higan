#include "../byuu.hpp"
#include "famicom.cpp"

vector<shared_pointer<Emulator::Instance>> emulators;
shared_pointer<Emulator::Instance> emulator;

namespace Emulator {

auto construct() -> void {
  #ifdef CORE_FC
  emulators.append(new Emulator::Famicom);
  #endif
}

auto Instance::load(const string& name, const vector<uint8_t>& data) -> void {
  game.name = name;
  game.data = data;

  auto system = higan::Node::System::create();
  system->setName(interface->name());
  auto configuration = higan::Node::serialize(system);
  interface->load(root, configuration);

  load();
  interface->power();
}

}
