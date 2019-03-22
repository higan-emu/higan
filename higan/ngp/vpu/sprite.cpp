auto VPU::renderSprite(uint8 lx, uint8 ly) -> bool {
  maybe<Sprite&> p;
  for(auto& s : sprites) {
    if(s.priority == 0) continue;

    s.x = s.hoffset;
    s.y = s.voffset;
    if(s.hchain && p) s.x += p->x;
    if(s.vchain && p) s.y += p->y;
    p = s;

    uint8 x = s.x + sprite.hscroll + 7 - lx;
    if(x >= 8) continue;  //out of range?
    uint8 y = s.y + sprite.vscroll + 7 - ly;
    if(y >= 8) continue;  //out of range?

    if(s.hflip == 1) x ^= 7;  //bit endian of tiledata is reversed
    if(s.vflip == 0) y ^= 7;

    if(uint2 index = characters[s.character][y][x]) {
      sprite.priority = s.priority;
      if(Model::NeoGeoPocket()) {
        sprite.output = sprite.palette[s.palette][index];
      }
      if(Model::NeoGeoPocketColor()) {
        if(screen.colorMode) {
          sprite.output = colors[sprite.colorCompatible + s.palette * 8 + sprite.palette[s.palette][index]];
        } else {
          sprite.output = colors[sprite.colorNative + s.code * 4 + index];
        }
      }
      return true;
    }
  }

  return false;
}
