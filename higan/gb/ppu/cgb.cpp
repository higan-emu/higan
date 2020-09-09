//BG attributes:
//0x80: 0 = OAM priority, 1 = BG priority
//0x40: vertical flip
//0x20: horizontal flip
//0x08: VRAM bank#
//0x07: palette#

//OB attributes:
//0x80: 0 = OBJ above BG, 1 = BG above OBJ
//0x40: vertical flip
//0x20: horizontal flip
//0x08: VRAM bank#
//0x07: palette#

auto PPU::readTileCGB(bool select, uint x, uint y, uint& attr, uint& data) -> void {
  uint tmaddr = 0x1800 + (select << 10);
  tmaddr += (((y >> 3) << 5) + (x >> 3)) & 0x03ff;

  uint tile = vram[0x0000 + tmaddr];
  attr = vram[0x2000 + tmaddr];

  uint tdaddr = attr & 0x08 ? 0x2000 : 0x0000;
  if(status.bgTiledataSelect == 0) {
    tdaddr += 0x1000 + ((int8)tile << 4);
  } else {
    tdaddr += 0x0000 + (tile << 4);
  }

  y &= 7;
  if(attr & 0x40) y ^= 7;
  tdaddr += y << 1;

  data  = vram[tdaddr++] << 0;
  data |= vram[tdaddr++] << 8;
  if(attr & 0x20) data = hflip(data);
}

auto PPU::scanlineCGB() -> void {
  px = 0;

  const uint Height = (status.obSize == 0 ? 8 : 16);
  sprites = 0;

  //find first ten sprites on this scanline
  for(uint n = 0; n < 40 * 4; n += 4) {
    Sprite& s = sprite[sprites];
    s.y = oam[n + 0] - 16;
    s.x = oam[n + 1] -  8;
    s.tile = oam[n + 2] & ~status.obSize;
    s.attr = oam[n + 3];

    s.y = status.ly - s.y;
    if(s.y >= Height) continue;

    if(s.attr & 0x40) s.y ^= (Height - 1);
    uint tdaddr = (s.attr & 0x08 ? 0x2000 : 0x0000) + (s.tile << 4) + (s.y << 1);
    s.data  = vram[tdaddr + 0] << 0;
    s.data |= vram[tdaddr + 1] << 8;
    if(s.attr & 0x20) s.data = hflip(s.data);

    if(++sprites == 10) break;
  }
}

auto PPU::runCGB() -> void {
  ob.color = 0;
  ob.palette = 0;
  ob.priority = 0;

  uint color = 0x7fff;
  runBackgroundCGB();
  if(latch.windowDisplayEnable) runWindowCGB();
  if(status.obEnable) runObjectsCGB();

  if(ob.palette == 0) {
    color = bg.color;
  } else if(bg.palette == 0) {
    color = ob.color;
  } else if(status.bgEnable == 0) {
    color = ob.color;
  } else if(bg.priority) {
    color = bg.color;
  } else if(ob.priority) {
    color = ob.color;
  } else {
    color = bg.color;
  }

  uint32* output = this->output + status.ly * 160 + px++;
  *output = color;
}

auto PPU::runBackgroundCGB() -> void {
  uint scrolly = (status.ly + status.scy) & 255;
  uint scrollx = (px + status.scx) & 255;
  uint tx = scrollx & 7;
  if(tx == 0 || px == 0) readTileCGB(status.bgTilemapSelect, scrollx, scrolly, background.attr, background.data);

  uint index = 0;
  index |= (background.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (background.data & (0x8000 >> tx)) ? 2 : 0;
  uint palette = ((background.attr & 0x07) << 2) + index;
  uint color = 0;
  color |= bgpd[(palette << 1) + 0] << 0;
  color |= bgpd[(palette << 1) + 1] << 8;
  color &= 0x7fff;

  bg.color = color;
  bg.palette = index;
  bg.priority = (background.attr & 0x80) >> 7;
}

auto PPU::runWindowCGB() -> void {
  if(status.ly < status.wy) return;
  if(px + 7 == status.wx) latch.wy++;

  uint scrolly = latch.wy - 1;
  uint scrollx = px + 7 - latch.wx;

  if(scrollx >= 160u) return;  //also matches underflow (scrollx < 0)
  uint tx = scrollx & 7;
  if(tx == 0 || px == 0) readTileCGB(status.windowTilemapSelect, scrollx, scrolly, window.attr, window.data);

  uint index = 0;
  index |= (window.data & (0x0080 >> tx)) ? 1 : 0;
  index |= (window.data & (0x8000 >> tx)) ? 2 : 0;
  uint palette = ((window.attr & 0x07) << 2) + index;
  uint color = 0;
  color |= bgpd[(palette << 1) + 0] << 0;
  color |= bgpd[(palette << 1) + 1] << 8;
  color &= 0x7fff;

  bg.color = color;
  bg.palette = index;
  bg.priority = (window.attr & 0x80) >> 7;
}

auto PPU::runObjectsCGB() -> void {
  //render backwards, so that first sprite has priority
  for(int n = sprites - 1; n >= 0; n--) {
    Sprite& s = sprite[n];

    int tx = px - s.x;
    if(tx < 0 || tx > 7) continue;

    uint index = 0;
    index |= (s.data & (0x0080 >> tx)) ? 1 : 0;
    index |= (s.data & (0x8000 >> tx)) ? 2 : 0;
    if(index == 0) continue;

    uint palette = ((s.attr & 0x07) << 2) + index;
    uint color = 0;
    color |= obpd[(palette << 1) + 0] << 0;
    color |= obpd[(palette << 1) + 1] << 8;
    color &= 0x7fff;

    ob.color = color;
    ob.palette = index;
    ob.priority = !(s.attr & 0x80);
  }
}
