struct Emulator {
  static auto construct() -> void;
  auto load(const string& name, const vector<uint8_t>& data) -> void;
  auto unload() -> void;
  virtual auto load() -> void = 0;
  virtual auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> = 0;
  virtual auto input(higan::Node::Input) -> void = 0;

  struct Game {
    string name;
    string manifest;
    vector<uint8_t> data;
  };

  shared_pointer<higan::Interface> interface;
  string name;
  string abbreviation;
  vector<string> extensions;

  higan::Node::Object root;
  Game game;

  struct Settings {
    string gamePath;
  } settings;
};

extern vector<shared_pointer<Emulator>> emulators;
extern shared_pointer<Emulator> emulator;
