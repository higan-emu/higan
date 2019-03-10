auto VPU::renderPlane(Plane& plane) -> maybe<uint12> {
  uint8 x = io.hcounter + plane.hscroll;
  uint8 y = io.vcounter + plane.vscroll;

  uint address = plane.address;
  address += (y >> 3) << 6;
  address += (x >> 3) << 1;

  x = (uint3)x;
  y = (uint3)y;

  uint16 attributes;
  attributes.byte(0) = scrollRAM[address + 0];
  attributes.byte(1) = scrollRAM[address + 1];

  uint9 character = attributes.bits(0,8);
  uint4 code      = attributes.bits(9,12);
  uint1 palette   = attributes.bit(13);
  uint1 vflip     = attributes.bit(14);
  uint1 hflip     = attributes.bit(15);

  if(hflip == 0) x ^= 7;
  if(vflip == 1) y ^= 7;

  address = character << 4;
  address += y << 1;

  uint16 tiledata;
  tiledata.byte(0) = characterRAM[address + 0];
  tiledata.byte(1) = characterRAM[address + 1];

  if(uint2 index = tiledata >> (x << 1)) {
    if(Model::NeoGeoPocket()) {
      if(index) return (uint12)plane.palette[palette][index];
    }
    if(Model::NeoGeoPocketColor()) {
      if(screen.colorMode) {
        return colorPalette[plane.colorCompatible + palette * 8 + plane.palette[palette][index]];
      } else {
        return colorPalette[plane.colorNative + code * 4 + index];
      }
    }
  }

  return {};
}
