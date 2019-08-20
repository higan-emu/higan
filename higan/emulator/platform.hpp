#pragma once

namespace higan {

enum class Event : uint {
  None,
  Step,
  Frame,
  Power,
  Serialize,
};

struct Platform {
  virtual auto attach(Node::Object) -> void {}
  virtual auto detach(Node::Object) -> void {}
  virtual auto open(Node::Object, string name, vfs::file::mode mode, bool required = false) -> shared_pointer<vfs::file> { return {}; }
  virtual auto event(Event) -> void {}
  virtual auto video(Node::Video, const uint32_t* data, uint pitch, uint width, uint height) -> void {}
  virtual auto audio(Node::Audio, const double* samples, uint channels) -> void {}
  virtual auto input(Node::Input) -> void {}
};

extern Platform* platform;

}

namespace higan::Core {
  auto PlatformAttach(Node::Object node) -> void { if(platform && node->name) platform->attach(node); }
  auto PlatformDetach(Node::Object node) -> void { if(platform && node->name) platform->detach(node); }
}
