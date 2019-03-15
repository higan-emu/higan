#include <ngp/ngp.hpp>

namespace higan::NeoGeoPocket {

Interface* interface = nullptr;

auto AbstractInterface::root() -> Node::Object {
  return system.node;
}

auto AbstractInterface::load(string tree) -> void {
  interface = this;
  system.load(Node::unserialize(tree));
}

auto AbstractInterface::unload() -> void {
  system.save();
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

}
