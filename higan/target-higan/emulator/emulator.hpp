struct Emulator : higan::Platform {
  higan::Node::Object root;

  //emulator.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto unload() -> void;
  auto main() -> void;
  auto quit() -> void;
  auto power(bool on) -> void;
  auto validateConfiguration(Markup::Node, Markup::Node) -> void;

  auto connected(string location) -> higan::Node::Port;

  //platform.cpp
  auto attach(higan::Node::Object) -> void override;
  auto detach(higan::Node::Object) -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto event(higan::Event) -> void override;
  auto video(higan::Node::Video, const uint32_t* data, uint pitch, uint width, uint height) -> void override;
  auto audio(higan::Node::Audio, const double* samples, uint channels) -> void override;
  auto input(higan::Node::Input) -> void override;

  //video.cpp
  auto videoUpdate() -> void;
  auto videoUpdateColors() -> void;
  auto videoUpdateShader() -> void;
  auto videoToggleFullscreen() -> void;

  //audio.cpp
  auto audioUpdate() -> void;
  auto audioUpdateEffects() -> void;

  //input.cpp
  auto inputUpdate() -> void;

  //states.cpp
  auto saveState(uint slot) -> bool;
  auto loadState(uint slot) -> bool;

  //status.cpp
  auto setMessage(const string& message = {}) -> void;
  auto setCaption(const string& caption = {}) -> void;

  struct System {
    string name;
    string data;
    string templates;
    bool power = false;
  } system;

  struct Events {
    bool power = false;
  } events;
};

extern Emulator emulator;
