auto Sprite::create(uint width, uint height) -> type& {
  this->width = width;
  this->height = height;
  pixels = new uint32[width * height]();
  return *this;
}

auto Sprite::destroy() -> type& {
  pixels = {};
  return *this;
}

auto Sprite::setPixels(const nall::image& image) -> type& {
  memory::copy(this->pixels.data(), width * height * sizeof(uint32), image.data(), image.size());
  return *this;
}

auto Sprite::setVisible(bool visible) -> type& {
  this->visible = visible;
  return *this;
}

auto Sprite::setPosition(int x, int y) -> type& {
  this->x = x;
  this->y = y;
  return *this;
}

auto Sprite::invert() -> type& {
  for(uint y : range(height)) {
    auto data = pixels.data() + y * width;
    for(uint x : range(width)) {
      auto pixel = data[x];
      pixel ^= 0xffffff;
      data[x] = pixel;
    }
  }
  return *this;
}
