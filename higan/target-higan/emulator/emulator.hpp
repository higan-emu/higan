struct Emulator : higan::Platform {
  Audio audio;

  //emulator.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto main() -> void;
  auto quit() -> void;
  auto power(bool on) -> void;

  auto connected(string location) -> bool;
  auto load(higan::Node::Peripheral) -> void;
  auto save(higan::Node::Peripheral) -> void;

  //platform.cpp
  auto attach(higan::Node) -> void override;
  auto detach(higan::Node) -> void override;
  auto open(higan::Node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto videoFrame(higan::Node::Video, const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(const double* samples, uint channels) -> void override;
  auto inputPoll(higan::Node::Input) -> void override;

  higan::Node root;

  struct System {
    string name;
    string data;
    string templates;
    bool power = false;
  } system;

  vector<shared_pointer<ViewportWindow>> viewports;
};

extern Emulator emulator;
