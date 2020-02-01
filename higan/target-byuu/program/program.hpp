struct Program : higan::Platform {
  auto create() -> void;
  auto main() -> void;
  auto quit() -> void;

  //platform.cpp
  auto attach(higan::Node::Object) -> void override;
  auto detach(higan::Node::Object) -> void override;
  auto open(higan::Node::Object, string name, vfs::file::mode mode, bool required) -> shared_pointer<vfs::file> override;
  auto event(higan::Event) -> void override;
  auto log(string_view message) -> void override;
  auto video(higan::Node::Screen, const uint32_t* data, uint pitch, uint width, uint height) -> void override;
  auto audio(higan::Node::Stream) -> void override;
  auto input(higan::Node::Input) -> void override;

  //load.cpp
  auto loadGame(shared_pointer<Emulator::Instance> emulator) -> void;
  auto unloadGame() -> void;

  vector<higan::Node::Screen> screens;
  vector<higan::Node::Stream> streams;
};

extern Program program;
