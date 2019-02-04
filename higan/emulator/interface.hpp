#pragma once

namespace higan {

struct Interface {
  struct Information {
    string manufacturer;
    string name;
    string extension;
    bool resettable = false;
  };

  struct Display {
    struct Type { enum : uint {
      CRT,
      LCD,
    };};
    uint id = 0;
    string name;
    uint type = 0;
    uint colors = 0;
    uint width = 0;
    uint height = 0;
    uint internalWidth = 0;
    uint internalHeight = 0;
    double aspectCorrection = 0;
  };

  struct Port {
    uint id;
    string name;
  };

  struct Device {
    uint id;
    string name;
  };

  struct Input {
    struct Type { enum : uint {
      Hat,
      Button,
      Trigger,
      Control,
      Axis,
      Rumble,
    };};

    uint type;
    string name;
  };

  virtual ~Interface() = default;

  //information
  virtual auto information() -> Information { return {}; }

  virtual auto display() -> Display { return {}; }
  virtual auto color(uint32 color) -> uint64 { return 0; }

  //game interface
  virtual auto loaded() -> bool { return false; }
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
  virtual auto root() -> Node { return {}; }
  virtual auto ports() -> vector<Port> { return {}; }
  virtual auto devices(uint port) -> vector<Device> { return {}; }
  virtual auto inputs(uint device) -> vector<Input> { return {}; }
  virtual auto connected(uint port) -> uint { return 0; }
  virtual auto connect(uint port, uint device) -> void {}
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

  //options
  virtual auto options() -> Settings& { static Setting<> noOptions; return noOptions; }

  auto hasOption(string name) -> bool { name.prepend("options/"); return (bool)options()[name]; }

  auto getOption(string name, string fallback = {}) -> string {
    name.prepend("options/");
    if(auto option = options()[name]) return option->value();
    return fallback;
  }

  auto setOption(string name, string value) -> bool {
    name.prepend("options/");
    if(auto option = options()[name]) return option->setValue(value);
    return false;
  }

  //properties
  virtual auto properties() -> Settings& { static Setting<> noProperties; return noProperties; }

  auto hasProperty(string name) -> bool { return (bool)properties()[name]; }

  auto getProperty(string name, string fallback = {}) -> string {
    if(auto property = properties()[name]) return property->value();
    return fallback;
  }

  auto setProperty(string name, string value) -> bool {
    if(auto property = properties()[name]) return property->setValue(value);
    return false;
  }
};

}
