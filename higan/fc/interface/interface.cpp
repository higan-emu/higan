#include <fc/fc.hpp>

namespace higan::Famicom {

Interface* interface = nullptr;

auto FamicomInterface::root() -> Node::Object {
  return system.node;
}

auto FamicomInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto FamicomInterface::unload() -> void {
  system.unload();
}

auto FamicomInterface::save() -> void {
  system.save();
}

auto FamicomInterface::power() -> void {
  system.power(false);
}

auto FamicomInterface::run() -> void {
  system.run();
}

auto FamicomInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto FamicomInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto FamicomInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

}
