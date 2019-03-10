auto VPU::renderSprite() -> maybe<uint12> {
  maybe<Sprite&> p;
  for(auto& s : sprites) {
    s.x = s.hoffset;
    s.y = s.voffset;
    if(s.hchain && p) s.x += p->x;
    if(s.vchain && p) s.y += p->y;
    p = s;

    uint8 x = s.x + sprite.hscroll + 7 - io.hcounter;
    if(x >= 8) continue;  //out of range?
    uint8 y = s.y + sprite.vscroll + 7 - io.vcounter;
    if(y >= 8) continue;  //out of range?

    if(s.hflip == 1) x ^= 7;  //bit endian of tiledata is reversed
    if(s.vflip == 0) y ^= 7;

    uint address = s.character << 4;
    address += y << 1;

    uint16 tiledata;
    tiledata.byte(0) = characterRAM[address + 0];
    tiledata.byte(1) = characterRAM[address + 1];

    if(uint2 index = tiledata >> (x << 1)) {
      if(Model::NeoGeoPocket()) {
        return (uint12)sprite.palette[s.palette][index];
      }
      if(Model::NeoGeoPocketColor()) {
        if(screen.colorMode) {
          return colorPalette[sprite.colorCompatible + s.palette * 8 + sprite.palette[s.palette][index]];
        } else {
          return colorPalette[sprite.colorNative + s.code * 4 + index];
        }
      }
    }
  }

  return {};
}
