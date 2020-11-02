auto MDEC::YUVtoRGB(queue<u8>& rgbBlock, i16 (*YnBlock)[8], i16 (*CrBlock)[8], i16 (*CbBlock)[8], uint startX, uint startY) -> void
{
  constexpr i16 rec601[3][3] = {
    { 256,  256,  256 },
    {   0,  -88,  454 },
    { 359, -183,    0 },
  };

  rgbBlock.resize(3*8*8);

  for(uint y : range(8)) {
    for(uint x : range(8)) {
      i16 Yd = YnBlock[y][x] + 128;
      i16 Cb = CbBlock[(startY + y) >> 1][(startX + x) >> 1];
      i16 Cr = CrBlock[(startY + y) >> 1][(startX + x) >> 1];

      i32 r32 = rec601[0][0] * Yd + rec601[1][0] * Cb + rec601[2][0] * Cr;
      i32 g32 = rec601[0][1] * Yd + rec601[1][1] * Cb + rec601[2][1] * Cr;
      i32 b32 = rec601[0][2] * Yd + rec601[1][2] * Cb + rec601[2][2] * Cr;

      //round to 8-bit unsigned integer
      u8 r = uclamp<8>((r32 + 128) >> 8);
      u8 g = uclamp<8>((g32 + 128) >> 8);
      u8 b = uclamp<8>((b32 + 128) >> 8);
      if(io.dataOutputSigned) {
        r ^= 0x80;
        g ^= 0x80;
        b ^= 0x80;
      }

      rgbBlock.write(r);
      rgbBlock.write(g);
      rgbBlock.write(b);
    }
  }
}
