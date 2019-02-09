#pragma once

namespace higan {

struct Platform {
  virtual auto attach(Node::Object) -> void {}
  virtual auto detach(Node::Object) -> void {}
  virtual auto open(Node::Object, string name, vfs::file::mode mode, bool required = false) -> vfs::shared::file { return {}; }
  virtual auto videoFrame(Node::Video, const uint32* data, uint pitch, uint width, uint height) -> void {}
  virtual auto audioFrame(Node::Audio, const double* samples, uint channels) -> void {}
  virtual auto inputPoll(Node::Input) -> void {}
  virtual auto inputRumble(uint port, uint device, uint input, bool enable) -> void {}
};

extern Platform* platform;

}

namespace higan::Core {
  auto PlatformAttach(Node::Object node) -> void { if(platform && node->name) platform->attach(node); }
  auto PlatformDetach(Node::Object node) -> void { if(platform && node->name) platform->detach(node); }
}
