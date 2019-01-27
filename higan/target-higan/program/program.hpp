struct Program : higan::Platform {
  auto construct(Arguments) -> void;
  auto main() -> void;
  auto quit() -> void;

  auto activate(shared_pointer<higan::Interface> interface) -> void;
  auto deactivate() -> void;
  auto power(bool) -> void;

  //platform.cpp
  auto path(uint id) -> string override;
  auto open(uint id, string name, vfs::file::mode mode, bool required) -> vfs::shared::file override;
  auto load(uint id, string name, string type, vector<string> options = {}) -> higan::Platform::Load override;
  auto videoFrame(const uint32* data, uint pitch, uint width, uint height) -> void override;
  auto audioFrame(const double* samples, uint channels) -> void override;
  auto inputPoll(uint port, uint device, uint input) -> int16 override;
  auto inputRumble(uint port, uint device, uint input, bool enable) -> void override;
  auto dipSettings(Markup::Node node) -> uint override;
  auto notify(string text) -> void override;

  struct Slot {
    uint id = 0;
    string location;
  };
  vector<Slot> slots;

  vector<string> gamePaths;
};

extern Program program;
