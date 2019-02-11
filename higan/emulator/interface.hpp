#pragma once

namespace higan {

struct Interface {
  virtual ~Interface() = default;

  //information
  virtual auto name() -> string { return {}; }

  //system interface
  virtual auto root() -> Node::Object { return {}; }
  virtual auto load(string tree = {}) -> void {}
  virtual auto power() -> void {}
  virtual auto run() -> void {}
  virtual auto save() -> void {}
  virtual auto unload() -> void {}

  //state functions
  virtual auto serialize() -> serializer { return {}; }
  virtual auto unserialize(serializer&) -> bool { return false; }

  //cheat functions
  virtual auto cheats(const vector<string>& = {}) -> void {}
};

}
