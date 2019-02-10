#pragma once

namespace higan {

struct Interface {
  virtual ~Interface() = default;

  //information
  virtual auto name() -> string { return {}; }
  virtual auto colors() -> uint32 { return 1; }
  virtual auto color(uint32 color) -> uint64 { return 0; }

  //system interface
  virtual auto root() -> Node::Object { return {}; }
  virtual auto load(string tree = {}) -> void {}
  virtual auto power() -> void {}
  virtual auto reset() -> void {}
  virtual auto run() -> void {}
  virtual auto save() -> void {}
  virtual auto unload() -> void {}

  //time functions
  virtual auto rtc() -> bool { return false; }
  virtual auto synchronize(uint64 timestamp = 0) -> void {}

  //state functions
  virtual auto serialize() -> serializer { return {}; }
  virtual auto unserialize(serializer&) -> bool { return false; }

  //cheat functions
  virtual auto cheats(const vector<string>& = {}) -> void {}
};

}
