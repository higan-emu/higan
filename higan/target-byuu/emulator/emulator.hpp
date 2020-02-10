struct Emulator {
  static auto construct() -> void;
  auto manifest() -> shared_pointer<vfs::file>;
  auto load(const string& location, const vector<uint8_t>& image) -> void;
  auto unload() -> void;
  virtual auto load() -> void = 0;
  virtual auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> = 0;
  virtual auto input(higan::Node::Input) -> void = 0;

  struct Game {
    string location;
    string manifest;
    vector<uint8_t> image;
  };

  shared_pointer<higan::Interface> interface;
  string name;
  string abbreviation;
  vector<string> extensions;

  higan::Node::Object root;
  Game game;

  struct Paths {
    string game;
    string bios;
  } path;

  //some handheld screens can be rotated
  uint rotation = 0;
};

extern vector<shared_pointer<Emulator>> emulators;
extern shared_pointer<Emulator> emulator;
