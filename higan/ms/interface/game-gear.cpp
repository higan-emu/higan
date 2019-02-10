auto GameGearInterface::name() -> string {
  return "Game Gear";
}

auto GameGearInterface::colors() -> uint32 {
  return 1 << 12;
}

auto GameGearInterface::color(uint32 color) -> uint64 {
  uint4 B = color >> 8;
  uint4 G = color >> 4;
  uint4 R = color >> 0;

  uint64 r = image::normalize(R, 4, 16);
  uint64 g = image::normalize(G, 4, 16);
  uint64 b = image::normalize(B, 4, 16);

  return r << 32 | g << 16 | b << 0;
}
