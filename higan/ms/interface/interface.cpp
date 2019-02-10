#include <ms/ms.hpp>

namespace higan::MasterSystem {

Interface* interface = nullptr;
#include "colecovision.cpp"
#include "sg-1000.cpp"
#include "sc-3000.cpp"
#include "master-system.cpp"
#include "game-gear.cpp"

auto AbstractInterface::initialize(string configuration) -> void {
  interface = this;
  return system.initialize(configuration);
}

auto AbstractInterface::terminate() -> void {
  return system.terminate();
}

auto AbstractInterface::root() -> Node::Object {
  return system.root;
}

auto AbstractInterface::power() -> void {
  system.power();
}

auto AbstractInterface::run() -> void {
  system.run();
}

auto AbstractInterface::save() -> void {
  system.save();
}

auto AbstractInterface::unload() -> void {
  save();
  system.unload();
}

auto AbstractInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto AbstractInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto AbstractInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

}
