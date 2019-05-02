//Graphics Processing Unit

auto MCD::GPU::step(uint clocks) -> void {
  if(!active) return;

  counter += clocks;
  while(counter >= period) {
    counter -= period;
    render(image.address, image.hdots);
    image.address += 8;
    if(!--image.vdots) {
      active = 0;
      irq.raise();
    }
  }
}

auto MCD::GPU::read(uint19 address) -> uint4 {
  static const uint table[] = {0, 4, 8, 12};
  uint lo = table[address & 3], hi = lo + 3;
  return mcd.wram[address >> 2].bits(lo, hi);
}

auto MCD::GPU::write(uint19 address, uint4 data) -> void {
  static const uint table[] = {0, 4, 8, 12};
  uint lo = table[address & 3], hi = lo + 3;
  mcd.wram[address >> 2].bits(lo, hi) = data;
}

auto MCD::GPU::render(uint19 address, uint9 width) -> void {
   uint8 stampShift = 11 + 4 + stamp.tile.size;
   uint8 mapShift = (4 << stamp.map.size) - stamp.tile.size;
  uint23 dotMask = !stamp.map.size ? 0x07ffff : 0x7fffff;

  uint24 x = mcd.wram[vector.address++] << 8;  //13.3 -> 13.11
  uint24 y = mcd.wram[vector.address++] << 8;  //13.3 -> 13.11

   int16 xstep = mcd.wram[vector.address++];
   int16 ystep = mcd.wram[vector.address++];

  while(width--) {
    if(stamp.repeat) {
      x &= dotMask;
      y &= dotMask;
    }

    uint4 output;
    if(!((x | y) & ~dotMask)) {
      uint16 stampData = mcd.wram[stamp.map.address + (x >> stampShift) + (y >> stampShift << mapShift)];
      if(uint19 stampIndex = stampData << 8) {
        stampData >>= 13;
        stampIndex |= lutCell[stampData | stamp.tile.size << 3 | x >> 10 & 0x30 | y >> 8 & 0xc0] << 6;
        stampIndex |= lutPixel[stampData | x >> 8 & 0x38 | y >> 5 & 0x1c0];
        output = read(stampIndex);
      }
    }

    uint4 input = read(address);
    switch(mcd.io.wramPriority) {
    case 0: output = output; break;
    case 1: output = input ? input : output; break;
    case 2: output = output ? output : input; break;
    case 3: output = input; break;
    }
    write(address, output);
    if(!(++address & 7)) address += (image.vcells + 1 << 6) - 8;

    x += xstep;
    y += ystep;
  }
}

auto MCD::GPU::start() -> void {
  if(mcd.io.wramMode) return;  //must be in 2mbit WRAM mode

  active = 1;
  period = 4 * 5 * image.hdots;
  counter = 0;

  image.address = (image.base << 1) + image.offset;
  vector.address = vector.base >> 1;
  stamp.map.address = stamp.map.base >> 1;
  if(stamp.map.size == 0 && stamp.tile.size == 0) stamp.map.address &= 0x1ff00;  // A9-A17
  if(stamp.map.size == 0 && stamp.tile.size == 1) stamp.map.address &= 0x1ffc0;  // A5-A17
  if(stamp.map.size == 1 && stamp.tile.size == 0) stamp.map.address &= 0x10000;  //    A17
  if(stamp.map.size == 1 && stamp.tile.size == 1) stamp.map.address &= 0x1c000;  //A15-A17
}

auto MCD::GPU::power(bool reset) -> void {
  for(uint n : range(0x100)) {
    auto mask = (n & 8) ? 3 : 1;
    auto row = n >> 6 & mask;
    auto col = n >> 4 & mask;
    auto tmp = col;
    if(n & 4) { col = col ^ mask; }
    if(n & 2) { col = col ^ mask; row = row ^ mask; }
    if(n & 1) { col = row ^ mask; row = tmp; }
    lutCell[n] = row + col * (mask + 1);
  }

  for(uint n : range(0x200)) {
    auto row = n >> 6 & 7;
    auto col = n >> 3 & 7;
    auto tmp = col;
    if(n & 4) { col = col ^ 7; }
    if(n & 2) { col = col ^ 7; row = row ^ 7; }
    if(n & 1) { col = row ^ 7; row = tmp; }
    lutPixel[n] = col + row * 8;
  }
}
