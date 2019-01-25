#include <ms/ms.hpp>

namespace higan::MasterSystem {

Options option;
Properties property;
#include "colecovision.cpp"
#include "sg-1000.cpp"
#include "sc-3000.cpp"
#include "master-system.cpp"
#include "game-gear.cpp"

auto AbstractInterface::loaded() -> bool {
  return system.loaded();
}

auto AbstractInterface::hashes() -> vector<string> {
  return {cartridge.hash()};
}

auto AbstractInterface::manifests() -> vector<string> {
  return {cartridge.manifest()};
}

auto AbstractInterface::titles() -> vector<string> {
  return {cartridge.title()};
}

auto AbstractInterface::save() -> void {
  system.save();
}

auto AbstractInterface::unload() -> void {
  save();
  system.unload();
}

auto AbstractInterface::power() -> void {
  system.power();
}

auto AbstractInterface::run() -> void {
  system.run();
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

auto AbstractInterface::options() -> Settings& {
  return option;
}

auto AbstractInterface::properties() -> Settings& {
  return property;
}

}
