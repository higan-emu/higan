#include <sfc/sfc.hpp>

namespace higan::SuperFamicom {

Interface* interface = nullptr;

auto SuperFamicomInterface::title() -> string {
  if(icd.node) {
    return icd.name();
  }

  if(bsmemory.node) {
    return {cartridge.name(), " + ", bsmemory.name()};
  }

  if(sufamiturboA.node && sufamiturboB.node) {
    return {sufamiturboA.name(), " + ", sufamiturboB.name()};
  }

  if(sufamiturboA.node) {
    return sufamiturboA.name();
  }

  if(sufamiturboB.node) {
    return sufamiturboB.name();
  }

  if(cartridge.node) {
    return cartridge.name();
  }

  return {};
}

auto SuperFamicomInterface::root() -> Node::Object {
  return system.node;
}

auto SuperFamicomInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto SuperFamicomInterface::unload() -> void {
  system.unload();
}

auto SuperFamicomInterface::save() -> void {
  system.save();
}

auto SuperFamicomInterface::power() -> void {
  system.power(false);
}

auto SuperFamicomInterface::run() -> void {
  system.run();
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
