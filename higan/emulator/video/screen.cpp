Screen::Screen(Node::Video node, uint width, uint height) {
  this->node = node;
  buffer = new uint32[width * height]();
  rotate = new uint32[width * height]();
  palette = new uint32[node->colors];
}

auto Screen::createSprite(uint width, uint height) -> shared_pointer<Sprite> {
  auto sprite = shared_pointer_make<Sprite>(width, height);
  sprites.append(sprite);
  return sprite;
}

auto Screen::removeSprite(shared_pointer<Sprite> sprite) -> bool {
  if(auto index = sprites.find(sprite)) return sprites.remove(*index), true;
  return false;
}

auto Screen::setPalette() -> void {
  for(auto index : range(node->colors)) {
    uint64 color = node->color(index);
    uint16 b = color.bits( 0,15);
    uint16 g = color.bits(16,31);
    uint16 r = color.bits(32,47);
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

    switch(depth) {
    case 24: palette[index] = a >>  8 << 24 | r >> 8 << 16 | g >> 8 <<  8 | b >> 8 << 0; break;
    case 30: palette[index] = a >> 14 << 30 | r >> 6 << 20 | g >> 6 << 10 | b >> 6 << 0; break;
    }
  }

  cached.palette = true;
}

auto Screen::setDepth(uint depth) -> void {
  cached.palette = false;
  this->depth = depth;
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
      uint32 mask = depth == 30 ? 0x40100401 : 0x01010101;
      for(uint x : range(width)) {
        auto a = *target;
        auto b = palette[*source++];
        *target++ = (a + b - ((a ^ b) & mask)) >> 1;
      }
    }
  }

  if(colorBleed) {
    uint32 mask = depth == 30 ? 0x40100401 : 0x01010101;
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

    uint32 opaqueAlpha = depth == 30 ? 0xc0000000 : 0xff000000;
    for(int y : range(sprite->height)) {
      for(int x : range(sprite->width)) {
        int pixelY = sprite->y + y;
        if(pixelY < 0 || pixelY >= height) continue;

        int pixelX = sprite->x + x;
        if(pixelX < 0 || pixelX >= width) continue;

        auto pixel = sprite->pixels[y * sprite->width + x];
        if(pixel) output[pixelY * width + pixelX] = opaqueAlpha | pixel;
      }
    }
  }

  platform->video(node, (const uint32_t*)output, width * sizeof(uint32), width, height);

  cached.width = width;
  cached.height = height;
}
