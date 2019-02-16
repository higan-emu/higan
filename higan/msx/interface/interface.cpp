#include <msx/msx.hpp>

namespace higan::MSX {

Interface* interface = nullptr;

auto MSXInterface::root() -> Node::Object {
  return system.node;
}

auto MSXInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto MSXInterface::unload() -> void {
  system.unload();
}

auto MSXInterface::save() -> void {
  system.save();
}

auto MSXInterface::power() -> void {
  system.power();
}

auto MSXInterface::run() -> void {
  system.run();
}

auto MSXInterface::serialize() -> serializer {
  system.runToSave();
  return system.serialize();
}

auto MSXInterface::unserialize(serializer& s) -> bool {
  return system.unserialize(s);
}

}
