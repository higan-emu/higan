auto MDEC::YtoGrayscale(queue<u8>& rgbBlock, i16 (*YnBlock)[8]) -> void {
  rgbBlock.resize(1*8*8);

  for(uint y : range(8)) {
    for(uint x : range(8)) {
      u8 g = YnBlock[y][x] + 128;
      if(io.dataOutputSigned) g ^= 0x80;
      rgbBlock.write(g);
    }
  }
}
