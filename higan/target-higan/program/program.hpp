struct Program : higan::Platform {
  //program.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto main() -> void;
  auto quit() -> void;
  auto power(bool on) -> void;

  //platform.cpp
  auto open(higan::Node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto videoFrame(higan::Node::Port::Video, const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(const double* samples, uint channels) -> void override;
  auto inputPoll(higan::Node::Input) -> void override;

  struct System {
    bool power = false;
  } system;

  vector<shared_pointer<ViewportWindow>> viewports;
};

extern Program program;
