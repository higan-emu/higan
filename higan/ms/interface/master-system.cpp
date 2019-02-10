auto MasterSystemInterface::name() -> string {
  return "Master System";
}

auto MasterSystemInterface::colors() -> uint32 {
  return 1 << 6;
}

auto MasterSystemInterface::color(uint32 color) -> uint64 {
  uint2 B = color >> 4;
  uint2 G = color >> 2;
  uint2 R = color >> 0;

  uint64 r = image::normalize(R, 2, 16);
  uint64 g = image::normalize(G, 2, 16);
  uint64 b = image::normalize(B, 2, 16);

  return r << 32 | g << 16 | b << 0;
}
