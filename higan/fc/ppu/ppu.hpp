struct PPU : Thread {
  Screen screen;

  inline auto rate() const -> uint { return Region::PAL() ? 5 : 4; }
  inline auto vlines() const -> uint { return Region::PAL() ? 312 : 262; }

  //ppu.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;

  auto scanline() -> void;
  auto frame() -> void;
  auto refresh() -> void;

  auto power(bool reset) -> void;

  //memory.cpp
  auto readCIRAM(uint11 addr) -> uint8;
  auto writeCIRAM(uint11 addr, uint8 data) -> void;

  auto readCGRAM(uint5 addr) -> uint8;
  auto writeCGRAM(uint5 addr, uint8 data) -> void;

  auto readIO(uint16 addr) -> uint8;
  auto writeIO(uint16 addr, uint8 data) -> void;

  //render.cpp
  auto enable() const -> bool;
  auto loadCHR(uint16 addr) -> uint8;

  auto renderPixel() -> void;
  auto renderSprite() -> void;
  auto renderScanline() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct IO {
    //internal
    uint8 mdr;

    uint1 field;
    uint lx = 0;
    uint ly = 0;

    uint8 busData;

    struct Union {
      uint19 data;
      BitRange<19> tileX      = {&data, 0, 4};
      BitRange<19> tileY      = {&data, 5, 9};
      BitRange<19> nametable  = {&data,10,11};
      BitRange<19> nametableX = {&data,10   };
      BitRange<19> nametableY = {&data,11   };
      BitRange<19> fineY      = {&data,12,14};
      BitRange<19> address    = {&data, 0,14};
      BitRange<19> addressLo  = {&data, 0, 7};
      BitRange<19> addressHi  = {&data, 8,14};
      BitRange<19> latch      = {&data,15   };
      BitRange<19> fineX      = {&data,16,18};
    } v, t;

    bool nmiHold = 0;
    bool nmiFlag = 0;

    //$2000
    uint vramIncrement = 1;
    uint spriteAddress = 0;
    uint bgAddress = 0;
    uint spriteHeight = 0;
    bool masterSelect = 0;
    bool nmiEnable = 0;

    //$2001
    bool grayscale = 0;
    bool bgEdgeEnable = 0;
    bool spriteEdgeEnable = 0;
    bool bgEnable = 0;
    bool spriteEnable = 0;
    uint3 emphasis;

    //$2002
    bool spriteOverflow = 0;
    bool spriteZeroHit = 0;

    //$2003
    uint8 oamAddress;
  } io;

  struct OAM {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint8 id = 64;
    uint8 y = 0xff;
    uint8 tile = 0xff;
    uint8 attr = 0xff;
    uint8 x = 0xff;

    uint8 tiledataLo = 0;
    uint8 tiledataHi = 0;
  };

  struct Latches {
    uint16 nametable;
    uint16 attribute;
    uint16 tiledataLo;
    uint16 tiledataHi;

    uint oamIterator = 0;
    uint oamCounter = 0;

    OAM oam[8];   //primary
    OAM soam[8];  //secondary
  } latch;

  uint8 ciram[2048];
  uint8 cgram[32];
  uint8 oam[256];

  uint32 buffer[256 * 262];
};

extern PPU ppu;
