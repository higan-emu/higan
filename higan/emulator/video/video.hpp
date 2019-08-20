#pragma once

namespace higan {

struct Video;
struct Screen;
struct Sprite;

struct Video {
  Video() = default;
  Video(const Video&) = delete;
  auto operator=(const Video&) = delete;

  auto reset(Interface* interface) -> void;
  auto append(Screen& screen) -> void;
  auto remove(Screen& screen) -> void;

  auto setPalette() -> void;
  auto setFormat(uint32 red, uint32 green, uint32 blue) -> void;
  auto setSaturation(double saturation) -> void;
  auto setGamma(double gamma) -> void;
  auto setLuminance(double luminance) -> void;
  auto setColorBleed(bool colorBleed) -> void;
  auto setInterframeBlending(bool interframeBlending) -> void;
  auto setRotateLeft(bool rotateLeft) -> void;

  vector<Screen*> screens;

private:
  Interface* interface = nullptr;
};

struct Screen {
  Screen() = default;
  Screen(const Screen&) = delete;
  auto operator=(const Screen&) = delete;

  auto create(Node::Video) -> void;
  auto destroy() -> void;

  auto append(Sprite& sprite) -> void;
  auto remove(Sprite& sprite) -> void;

  auto setPalette() -> void;
  auto setFormat(uint32 red, uint32 green, uint32 blue) -> void;
  auto setSaturation(double saturation) -> void;
  auto setGamma(double gamma) -> void;
  auto setLuminance(double luminance) -> void;
  auto setColorBleed(bool colorBleed) -> void;
  auto setInterframeBlending(bool interframeBlending) -> void;
  auto setRotateLeft(bool rotateLeft) -> void;

  auto refresh(uint32* input, uint pitch, uint width, uint height) -> void;

private:
  Node::Video node;

  unique_pointer<uint32[]> buffer;
  unique_pointer<uint32[]> rotate;
  unique_pointer<uint32[]> palette;
  vector<Sprite*> sprites;

  struct Channel {
    uint count;
    uint first;
  };
  //default: A8R8G8B8
  Channel alpha{8, 24};
  Channel red  {8, 16};
  Channel green{8,  8};
  Channel blue {8,  0};

  double saturation = 1.0;
  double gamma = 1.0;
  double luminance = 1.0;

  bool colorBleed = false;
  bool interframeBlending = false;
  bool rotateLeft = false;

  struct Cached {
    bool palette = false;
    uint width = 0;
    uint height = 0;
  } cached;
};

struct Sprite {
  using type = Sprite;

  Sprite() = default;
  Sprite(const Sprite&) = delete;
  auto operator=(const Sprite&) = delete;

  auto create(uint width, uint height) -> type&;
  auto destroy() -> type&;

  auto setPixels(const nall::image& image) -> type&;
  auto setVisible(bool visible) -> type&;
  auto setPosition(int x, int y) -> type&;

  auto invert() -> type&;

private:
  uint width;
  uint height;
  unique_pointer<uint32[]> pixels;

  bool visible = false;
  int x = 0;
  int y = 0;

  friend class Screen;
};

extern Video video;

}
