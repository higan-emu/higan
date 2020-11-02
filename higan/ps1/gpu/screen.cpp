auto GPU::setScreenColors(uint1 colorDepth) -> void
{
  switch(colorDepth) {
  case 0: //15bpp
    screen->colors(1 << 16, [&](uint32 color) -> uint64 {
      u64 a = 65535;
      u64 r = image::normalize(color >>  0 & 31, 5, 16);
      u64 g = image::normalize(color >>  5 & 31, 5, 16);
      u64 b = image::normalize(color >> 10 & 31, 5, 16);
      return a << 48 | r << 32 | g << 16 | b << 0;
    });
    refreshOutput = [&](void) -> u32 {
      constexpr u32 bytesPerPixel = 16 / 8;
      constexpr u32 pitch = 1024 * 2 / bytesPerPixel;
      u32 source = 0;
      u32 target = 0;
      for([[maybe_unused]] uint y : range(512)) {
        for([[maybe_unused]] uint x : range(pitch)) {
          u32 data = vram.readHalf(source++ << 1);
          output[target++] = data & 0x7fff;
        }
      }
      return bytesPerPixel;
    };
    break;
  case 1: //24bpp
    screen->colors(1 << 24, [&](uint32 color) -> uint64 {
      u64 a = 65535;
      u64 r = image::normalize(color >>  0 & 255, 8, 16);
      u64 g = image::normalize(color >>  8 & 255, 8, 16);
      u64 b = image::normalize(color >> 16 & 255, 8, 16);
      return a << 48 | r << 32 | g << 16 | b << 0;
    });
    refreshOutput = [&](void) -> u32 {
      constexpr u32 bytesPerPixel = 24 / 8;
      constexpr u32 pitch = 1024 * 2 / bytesPerPixel;
      u32 source = 0;
      u32 target = 0;
      for([[maybe_unused]] uint y : range(512)) {
        for([[maybe_unused]] uint x : range(pitch)) {
           u8 r = vram.readByte(source++);
           u8 g = vram.readByte(source++);
           u8 b = vram.readByte(source++);
          u32 data = (r << 0) | (g << 8) | (b << 16);
          output[target++] = data;
        }
        source += 2;
      }
      return bytesPerPixel;
    };
    break;
  }
}
