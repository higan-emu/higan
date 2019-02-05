#pragma once

namespace higan {

struct Interface {
  virtual ~Interface() = default;

  //information
  virtual auto name() -> string { return {}; }
  virtual auto colors() -> uint32 { return 1; }
  virtual auto color(uint32 color) -> uint64 { return 0; }

  //game interface
  virtual auto hashes() -> vector<string> { return {}; }
  virtual auto manifests() -> vector<string> { return {}; }
  virtual auto titles() -> vector<string> { return {}; }
  virtual auto load() -> bool { return false; }
  virtual auto load(uint slot) -> bool { return false; }
  virtual auto save() -> void {}
  virtual auto unload() -> void {}

  //system interface
  virtual auto initialize(string configuration = {}) -> void {}
  virtual auto terminate() -> void {}
  virtual auto root() -> Node::Object { return {}; }
  virtual auto power() -> void {}
  virtual auto reset() -> void {}
  virtual auto run() -> void {}

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
