#include <ms/ms.hpp>

namespace higan::MasterSystem {

Interface* interface = nullptr;

auto AbstractInterface::root() -> Node::Object {
  return system.root;
}

auto AbstractInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
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
