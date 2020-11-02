auto MDEC::decodeRLE(i16* mb, u8* qt) -> bool {
  static constexpr u8 zagzig[] = {
     0,  1,  8, 16,  9,  2,  3, 10,
    17, 24, 32, 25, 18, 11,  4,  5,
    12, 19, 26, 33, 40, 48, 41, 34,
    27, 20, 13,  6,  7, 14, 21, 28,
    35, 42, 49, 56, 57, 50, 43, 36,
    29, 22, 15, 23, 30, 37, 44, 51,
    58, 59, 52, 45, 38, 31, 39, 46,
    53, 60, 61, 54, 47, 55, 62, 63
  };
  auto& fifo = this->fifo.mdecIn;

  auto setMB = [&](int10 reference, int12 coefficient) -> void {
    if(currentQualityFactor == 0) {
      int12 value = reference * 2;
      mb[currentMatrixID] = value;
    } else {
      int12 value = reference * coefficient;
      mb[zagzig[currentMatrixID]] = value;
    }
  };

  //decode DCT block
  if(currentMatrixID == 0) {
    int10 DC_reference;
    uint6 qualityFactor;
    do {
      if(fifo.empty()) return false;
      uint16 data = fifo.read();
      DC_reference  = data.bit(0,  9);
      qualityFactor = data.bit(10,15);
    } while(qualityFactor == 63);

    currentQualityFactor = qualityFactor;
    setMB(DC_reference, qt[currentMatrixID]);
    currentMatrixID++;
  }

  //decode RLE block
  while(fifo.size() > 0) {
    uint16 data = fifo.read();
    int10 AC_reference = data.bit(0,  9);
    uint6 numZeroBytes = data.bit(10,15);
    while(numZeroBytes != 0 && currentMatrixID != 0) {
      setMB(0, 0);
      currentMatrixID++;
      numZeroBytes--;
    }
    if(currentMatrixID == 0) break;
    setMB(AC_reference, (qt[currentMatrixID] * currentQualityFactor + 4) >> 3);
    currentMatrixID++;
  }

  return currentMatrixID == 0;
}
