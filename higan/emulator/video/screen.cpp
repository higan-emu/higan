auto Screen::create(Node::Video node) -> void {
  this->node = node;
  buffer = new uint32[node->width * node->height]();
  rotate = new uint32[node->width * node->height]();
  palette = new uint32[node->colors];
  video.append(*this);
}

auto Screen::destroy() -> void {
  video.remove(*this);
}

auto Screen::append(Sprite& sprite) -> void {
  if(sprites.find(&sprite)) return;
  sprites.append(&sprite);
}

auto Screen::remove(Sprite& sprite) -> void {
  sprites.removeByValue(&sprite);
}

auto Screen::setPalette() -> void {
  for(uint index : range(node->colors)) {
    uint64 color = node->color(index);
    uint16 b = color.bit( 0,15);
    uint16 g = color.bit(16,31);
    uint16 r = color.bit(32,47);
    uint16 a = 0xffff;

    if(saturation != 1.0) {
      uint16 grayscale = uclamp<16>((r + g + b) / 3);
      double inverse = max(0.0, 1.0 - saturation);
      r = uclamp<16>(r * saturation + grayscale * inverse);
      g = uclamp<16>(g * saturation + grayscale * inverse);
      b = uclamp<16>(b * saturation + grayscale * inverse);
    }

    if(gamma != 1.0) {
      double reciprocal = 1.0 / 32767.0;
      r = r > 32767 ? r : uint16(32767 * pow(r * reciprocal, gamma));
      g = g > 32767 ? g : uint16(32767 * pow(g * reciprocal, gamma));
      b = b > 32767 ? b : uint16(32767 * pow(b * reciprocal, gamma));
    }

    if(luminance != 1.0) {
      r = uclamp<16>(r * luminance);
      g = uclamp<16>(g * luminance);
      b = uclamp<16>(b * luminance);
    }

    a >>= 16 - alpha.count;
    r >>= 16 - red.count;
    g >>= 16 - green.count;
    b >>= 16 - blue.count;

    palette[index] = a << alpha.first | r << red.first | g << green.first | b << blue.first;
  }

  cached.palette = true;
}

auto Screen::setFormat(uint32 red, uint32 green, uint32 blue) -> void {
  cached.palette = false;
  uint32 alpha = ~(red | green | blue);
  this->alpha = {bit::count(alpha), bit::first(alpha)};
  this->red   = {bit::count(red),   bit::first(red)};
  this->green = {bit::count(green), bit::first(green)};
  this->blue  = {bit::count(blue),  bit::first(blue)};
}

auto Screen::setSaturation(double saturation) -> void {
  cached.palette = false;
  this->saturation = saturation;
}

auto Screen::setGamma(double gamma) -> void {
  cached.palette = false;
  this->gamma = gamma;
}

auto Screen::setLuminance(double luminance) -> void {
  cached.palette = false;
  this->luminance = luminance;
}

auto Screen::setColorBleed(bool colorBleed) -> void {
  this->colorBleed = colorBleed;
}

auto Screen::setInterframeBlending(bool interframeBlending) -> void {
  this->interframeBlending = interframeBlending;
}

auto Screen::setRotateLeft(bool rotateLeft) -> void {
  this->rotateLeft = rotateLeft;
}

auto Screen::refresh(uint32* input, uint pitch, uint width, uint height) -> void {
  if(!cached.palette) setPalette();

  auto output = buffer.data();
  pitch >>= 2;  //bytes to words

  for(uint y : range(height)) {
    auto source = input + y * pitch;
    auto target = output + y * width;

    if(!interframeBlending || width != cached.width || height != cached.height) {
      for(uint x : range(width)) {
        auto color = palette[*source++];
        *target++ = color;
      }
    } else {
      uint32 mask = 1 << alpha.first | 1 << red.first | 1 << green.first | 1 << blue.first;
      for(uint x : range(width)) {
        auto a = *target;
        auto b = palette[*source++];
        *target++ = (a + b - ((a ^ b) & mask)) >> 1;
      }
    }
  }

  if(colorBleed) {
    uint32 mask = 1 << alpha.first | 1 << red.first | 1 << green.first | 1 << blue.first;
    for(uint y : range(height)) {
      auto target = output + y * width;
      for(uint x : range(width)) {
        auto a = target[x];
        auto b = target[x + (x != width - 1)];
        target[x] = (a + b - ((a ^ b) & mask)) >> 1;
      }
    }
  }

  if(rotateLeft) {
    for(uint y : range(height)) {
      auto source = output + y * width;
      for(uint x : range(width)) {
        auto target = rotate.data() + (width - 1 - x) * height + y;
        *target = *source++;
      }
    }
    output = rotate.data();
    swap(width, height);
  }

  for(auto& sprite : sprites) {
    if(!sprite->visible) continue;

    uint32 alpha = (1 << this->alpha.count) - 1 << this->alpha.first;
    for(int y : range(sprite->height)) {
      for(int x : range(sprite->width)) {
        int pixelY = sprite->y + y;
        if(pixelY < 0 || pixelY >= height) continue;

        int pixelX = sprite->x + x;
        if(pixelX < 0 || pixelX >= width) continue;

        auto pixel = sprite->pixels[y * sprite->width + x];
        if(pixel) output[pixelY * width + pixelX] = alpha | pixel;
      }
    }
  }

  platform->video(node, (const uint32_t*)output, width * sizeof(uint32), width, height);

  cached.width = width;
  cached.height = height;
}
