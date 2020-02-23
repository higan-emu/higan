struct Emulator {
  static auto construct() -> void;
  auto manifest() -> shared_pointer<vfs::file>;
  auto load(const string& location, const vector<uint8_t>& image) -> void;
  auto save() -> void;
  auto unload() -> void;
  auto setBoolean(const string& name, bool value) -> bool;
  auto setOverscan(bool value) -> bool;
  auto error(const string& text) -> void;
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
  vector<string> extensions;

  higan::Node::Object root;
  Game game;

  struct Configuration {
    bool visible = true;  //whether or not to show this emulator in the load menu
    string game;          //the most recently used folder for games for each emulator core
    string bios;          //the location of required BIOS files (for systems that have one)
  } configuration;

  struct Latch {
    uint width = 0;
    uint height = 0;
    uint rotation = 0;
  } latch;
};

extern vector<shared_pointer<Emulator>> emulators;
extern shared_pointer<Emulator> emulator;
