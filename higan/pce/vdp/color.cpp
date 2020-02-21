auto VDP::color(uint32 color) -> uint64 {
  uint3 B = color.bit(0,2);
  uint3 R = color.bit(3,5);
  uint3 G = color.bit(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}
