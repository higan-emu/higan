//Macroblock Decoder Unit

struct MDEC : Thread {
  Node::Component node;

  //mdec.cpp
  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto power(bool reset) -> void;

  //io.cpp
  auto readByte(u32 address) -> u8;
  auto readHalf(u32 address) -> u16;
  auto readWord(u32 address) -> u32;
  auto writeByte(u32 address, u8 data) -> void;
  auto writeHalf(u32 address, u16 data) -> void;
  auto writeWord(u32 address, u32 data) -> void;
  auto readDMA() -> u32;
  auto writeDMA(u32 value) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  //macroblock.cpp
  auto decodeMacroBlock(const vector<uint>& md_ids) -> bool;

  //rle.cpp
  auto decodeRLE(i16* mb, u8* qt) -> bool;

  //idct.cpp
  auto IDCT(i16 (*f)[8], i16* F) -> void;

  //yuvtorgb.cpp
  auto YUVtoRGB(queue<u8>& rgbBlock, i16 (*YnBlock)[8], i16 (*CrBlock)[8], i16 (*CbBlock)[8], uint startX, uint startY) -> void;

  //ytograyscale.cpp
  auto YtoGrayscale(queue<u8>& rgbBlock, i16 (*YnBlock)[8]) -> void;

  struct IO {
     uint3 currentBlock;
     uint1 enableDataOut;
     uint1 enableDataIn;
     uint3 command;

    //MDEC(1): decode macroblocks
     uint2 dataOutputDepth;
     uint1 dataOutputSigned;
     uint1 dataOutputBit15;
    uint16 numOfParamWords;

    //MDEC(2): set quant tables
     uint1 colorQuantTable;
  } io;

  struct {
    queue<u16> mdecIn;
    vector<queue<u8>> mdecOut;
    u32 counter;
  } fifo;

  uint6 currentMatrixID = 0;
  uint6 currentQualityFactor = 0;

   u8 quantTable[2][64];    //Y, C
  i16 scaleTable[8][8];
  i16 macroBlock[6][64];    //Y1, Y2, Y3, Y4, Cr, Cb
  i16 ycbcrBlock[6][8][8];  //Y1, Y2, Y3, Y4, Cr, Cb
};

extern MDEC mdec;
