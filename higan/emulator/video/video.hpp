#pragma once

namespace higan {

struct Video;
struct Sprite;
struct Screen;

struct Video {
  auto reset(Interface* interface) -> void;
  auto createScreen(Node::Video, uint width, uint height) -> shared_pointer<Screen>;
  auto removeScreen(shared_pointer<Screen>) -> bool;

private:
  Interface* interface = nullptr;
  vector<shared_pointer<Screen>> screens;
};

struct Sprite {
  Sprite(uint width, uint height);

  auto setPixels(const nall::image& image) -> void;
  auto setVisible(bool visible) -> void;
  auto setPosition(int x, int y) -> void;

private:
  const uint width;
  const uint height;
  unique_pointer<uint32[]> pixels;

  bool visible = false;
  int x = 0;
  int y = 0;

  friend class Screen;
};

struct Screen {
  Screen(Node::Video node, uint width, uint height);

  auto createSprite(uint width, uint height) -> shared_pointer<Sprite>;
  auto removeSprite(shared_pointer<Sprite>) -> bool;

  auto setPalette() -> void;
  auto setDepth(uint depth) -> void;
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
  vector<shared_pointer<Sprite>> sprites;

  uint depth = 24;
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

extern Video video;

}
