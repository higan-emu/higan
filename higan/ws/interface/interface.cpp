#include <ws/ws.hpp>

namespace higan::WonderSwan {

Interface* interface = nullptr;

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
