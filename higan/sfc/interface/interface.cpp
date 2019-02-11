#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

Interface* interface = nullptr;

auto SuperFamicomInterface::name() -> string {
  return "Super Famicom";
}

auto SuperFamicomInterface::root() -> Node::Object {
  return system.root;
}

auto SuperFamicomInterface::load(string tree) -> void {
  auto from = Node::unserialize(tree);
  system.load(from);
}

auto SuperFamicomInterface::power() -> void {
  system.power(false);
}

auto SuperFamicomInterface::run() -> void {
  system.run();
}

auto SuperFamicomInterface::save() -> void {
  system.save();
}

auto SuperFamicomInterface::unload() -> void {
  save();
  system.unload();
}

auto SuperFamicomInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto SuperFamicomInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto SuperFamicomInterface::cheats(const vector<string>& list) -> void {
  cheat.reset();
  #if defined(CORE_GB)
  if(cartridge.has.ICD) return GameBoy::cheat.assign(list);
  #endif
  cheat.assign(list);
}

}
