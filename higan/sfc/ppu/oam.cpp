auto PPU::OAM::read(uint10 address) -> uint8 {
  uint8 data;

  if(!address.field(9)) {
    uint7 n = address >> 2;  //object#
    address &= 3;
    if(address == 0) return object[n].x.range(0,7);
    if(address == 1) return object[n].y;
    if(address == 2) return object[n].character;
    data.field(0  ) = object[n].nameselect;
    data.range(1,3) = object[n].palette;
    data.range(4,5) = object[n].priority;
    data.field(6  ) = object[n].hflip;
    data.field(7  ) = object[n].vflip;
    return data;
  }

  uint7 n = (uint5)address << 2;  //object#
  data.field(0) = object[n + 0].x.field(8);
  data.field(1) = object[n + 0].size;
  data.field(2) = object[n + 1].x.field(8);
  data.field(3) = object[n + 1].size;
  data.field(4) = object[n + 2].x.field(8);
  data.field(5) = object[n + 2].size;
  data.field(6) = object[n + 3].x.field(8);
  data.field(7) = object[n + 3].size;
  return data;
}

auto PPU::OAM::write(uint10 address, uint8 data) -> void {
  if(!address.field(9)) {
    uint7 n = address >> 2;  //object#
    address &= 3;
    if(address == 0) { object[n].x.range(0,7) = data; return; }
    if(address == 1) { object[n].y = data; return; }
    if(address == 2) { object[n].character = data; return; }
    object[n].nameselect = data.field(0  );
    object[n].palette    = data.range(1,3);
    object[n].priority   = data.range(4,5);
    object[n].hflip      = data.field(6  );
    object[n].vflip      = data.field(7  );
  } else {
    uint7 n = (uint5)address << 2;  //object#
    object[n + 0].x.field(8) = data.field(0);
    object[n + 0].size       = data.field(1);
    object[n + 1].x.field(8) = data.field(2);
    object[n + 1].size       = data.field(3);
    object[n + 2].x.field(8) = data.field(4);
    object[n + 2].size       = data.field(5);
    object[n + 3].x.field(8) = data.field(6);
    object[n + 3].size       = data.field(7);
  }
}

auto PPU::OAM::Object::width() const -> uint {
  if(size == 0) {
    static const uint width[] = { 8,  8,  8, 16, 16, 32, 16, 16};
    return width[ppu.obj.io.baseSize];
  } else {
    static const uint width[] = {16, 32, 64, 32, 64, 64, 32, 32};
    return width[ppu.obj.io.baseSize];
  }
}

auto PPU::OAM::Object::height() const -> uint {
  if(size == 0) {
    if(ppu.obj.io.interlace && ppu.obj.io.baseSize >= 6) return 16;  //hardware quirk
    static const uint height[] = { 8,  8,  8, 16, 16, 32, 32, 32};
    return height[ppu.obj.io.baseSize];
  } else {
    static const uint height[] = {16, 32, 64, 32, 64, 64, 64, 32};
    return height[ppu.obj.io.baseSize];
  }
}
