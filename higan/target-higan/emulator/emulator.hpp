struct Emulator : higan::Platform {
  higan::Node::Object root;
  Audio audio;

  //emulator.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto main() -> void;
  auto quit() -> void;
  auto power(bool on) -> void;

  auto connected(string location) -> bool;

  //platform.cpp
  auto attach(higan::Node::Object) -> void override;
  auto detach(higan::Node::Object) -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto videoFrame(higan::Node::Video, const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(higan::Node::Audio, const double* samples, uint channels) -> void override;
  auto inputPoll(higan::Node::Input) -> void override;

  struct System {
    string name;
    string data;
    string templates;
    bool power = false;
  } system;

  vector<shared_pointer<ViewportWindow>> viewports;
};

extern Emulator emulator;
