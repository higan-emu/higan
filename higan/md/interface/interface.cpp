#include <md/md.hpp>

namespace higan::MegaDrive {

Interface* interface = nullptr;

auto MegaDriveInterface::root() -> Node::Object {
  return system.node;
}

auto MegaDriveInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto MegaDriveInterface::unload() -> void {
  system.unload();
}

auto MegaDriveInterface::save() -> void {
  system.save();
}

auto MegaDriveInterface::power() -> void {
  system.power(false);
}

auto MegaDriveInterface::run() -> void {
  system.run();
}

auto MegaDriveInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto MegaDriveInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

auto MegaDriveInterface::cheats(const vector<string>& list) -> void {
  cheat.assign(list);
}

}
