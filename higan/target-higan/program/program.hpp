struct Program : higan::Platform {
  //program.cpp
  auto create(shared_pointer<higan::Interface>, string location) -> void;
  auto main() -> void;
  auto power() -> void;

  //platform.cpp
  auto open(higan::Node node, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto videoFrame(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
};

extern Program program;
