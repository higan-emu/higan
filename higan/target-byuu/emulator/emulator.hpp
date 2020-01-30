namespace Emulator {

struct Instance {
  auto load(const string& name, const vector<uint8_t>& data) -> void;
  virtual auto load() -> void = 0;
  virtual auto load(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> = 0;

  struct Port {
    string name;
  };

  struct Game {
    string name;
    string manifest;
    vector<uint8_t> data;
  };

  shared_pointer<higan::Interface> interface;
  string name;
  string abbreviation;
  vector<string> extensions;
  vector<Port> ports;

  higan::Node::Object root;
  Game game;
};

auto construct() -> void;

struct Famicom : Instance {
  Famicom();
  auto load() -> void;
  auto load(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file>;
};

}

extern vector<shared_pointer<Emulator::Instance>> emulators;
extern shared_pointer<Emulator::Instance> emulator;
