struct Emulator : higan::Platform {
  higan::Node::Object root;
  Audio sound;

  //emulator.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto main() -> void;
  auto quit() -> void;
  auto power(bool on) -> void;

  auto connected(string location) -> higan::Node::Port;

  //platform.cpp
  auto attach(higan::Node::Object) -> void override;
  auto detach(higan::Node::Object) -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto video(higan::Node::Video, const uint32_t* data, uint pitch, uint width, uint height) -> void override;
  auto audio(higan::Node::Audio, const double* samples, uint channels) -> void override;
  auto input(higan::Node::Input) -> void override;

  struct System {
    string name;
    string data;
    string templates;
    bool power = false;
  } system;

  vector<shared_pointer<ViewportWindow>> viewports;
};

extern Emulator emulator;
