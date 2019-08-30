#include <md/md.hpp>

namespace higan::MegaDrive {

Interface* interface = nullptr;

auto MegaDriveInterface::game() -> string {
  if(expansion.node && (!cartridge.node || !cartridge.bootable())) {
    return expansion.name();
  }

  if(cartridge.node && cartridge.bootable()) {
    return cartridge.name();
  }

  return "(no cartridge connected)";
}

auto MegaDriveInterface::root() -> Node::Object {
  return system.node;
}

auto MegaDriveInterface::load(Node::Object& root, string tree) -> void {
  interface = this;
  system.load(root, Node::unserialize(tree));
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

}
