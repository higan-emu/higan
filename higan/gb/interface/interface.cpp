#include <gb/gb.hpp>

namespace higan::GameBoy {

Interface* interface = nullptr;
SuperGameBoyInterface* superGameBoy = nullptr;

auto AbstractInterface::root() -> Node::Object {
  return system.node;
}

auto AbstractInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto AbstractInterface::unload() -> void {
  system.unload();
}

auto AbstractInterface::save() -> void {
  system.save();
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

}
