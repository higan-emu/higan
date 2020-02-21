//Hudson Soft HuC6270: Video Display Controller

struct VDC {
  inline auto irqLine() const -> bool { return irq.line; }

  //vdc.cpp
  auto prologue(uint16 vcounter) -> void;
  auto render(uint16 vcounter) -> void;
  auto epilogue(uint16 vcounter) -> void;
  auto read(uint2 address) -> uint8;
  auto write(uint2 address, uint8 data) -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint9 output[512];

  struct VRAM {
    //vdc.cpp
    inline auto read(uint16 address) const -> uint16;
    inline auto write(uint16 address, uint16 data) -> void;

    uint16 memory[0x8000];

    uint16 addressRead;
    uint16 addressWrite;
    uint16 addressIncrement;

    uint16 dataRead;
    uint16 dataWrite;
  } vram;

  struct SATB {
    //vdc.cpp
    inline auto read(uint8 address) const -> uint16;
    inline auto write(uint8 address, uint16 data) -> void;

    uint16 memory[0x100];
  } satb;

  struct IRQ {
    enum class Line : uint {
      Collision,
      Overflow,
      Coincidence,
      Vblank,
      TransferVRAM,
      TransferSATB,
    };

    struct Source {
      uint1 enable;
      uint1 pending;
    };

    //irq.cpp
    auto poll() -> void;
    auto raise(Line) -> void;
    auto lower() -> void;

    uint1 line;

    Source collision;
    Source overflow;
    Source coincidence;
    Source vblank;
    Source transferVRAM;
    Source transferSATB;
  } irq;

  struct DMA {
    maybe<VDC&> vdc;

    //dma.cpp
    auto step(uint clocks) -> void;
    auto vramStart() -> void;
    auto satbStart() -> void;
    auto satbQueue() -> void;

     uint1 sourceIncrementMode;
     uint1 targetIncrementMode;
     uint1 satbRepeat;
    uint16 source;
    uint16 target;
    uint16 length;
    uint16 satbSource;

     uint1 vramActive;
     uint1 satbActive;
     uint1 satbPending;
     uint8 satbOffset;
  } dma;

  struct Timing {
     uint5 horizontalSyncWidth;
     uint7 horizontalDisplayStart;
     uint7 horizontalDisplayLength;
     uint7 horizontalDisplayEnd;

     uint5 verticalSyncWidth;
     uint8 verticalDisplayStart;
     uint9 verticalDisplayLength;
     uint8 verticalDisplayEnd;

    uint16 hoffset;
    uint16 voffset;

    uint16 hstart;
    uint16 vstart;

    uint16 hlength;
    uint16 vlength;
  } timing;

  struct IO {
     uint5 address;

    //$0005  CR (W)
     uint2 externalSync;
     uint2 displayOutput;
     uint1 dramRefresh;

    //$0006  RCR
    uint10 coincidence;

    //$0009  MWR
     uint2 vramAccess;
     uint2 spriteAccess;
     uint1 cgMode;
  } io;

  struct Background {
    maybe<VDC&> vdc;

    //background.cpp
    auto scanline(uint16 y) -> void;
    auto render(uint16 y) -> void;

     uint1 enable;
    uint10 hscroll;
     uint9 vscroll;
     uint9 vcounter;
     uint8 width;
     uint8 height;

    uint10 hoffset;
     uint9 voffset;

    struct Output {
      uint4 color;
      uint4 palette;
    } output[512];
  } background;

  struct Object {
    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint10 y;
    uint10 x;
     uint1 mode;
    uint10 pattern;
     uint4 palette;
     uint1 priority;
     uint8 width;
     uint8 height;
     uint1 hflip;
     uint1 vflip;
     uint1 first;
  };

  struct Sprite {
    maybe<VDC&> vdc;

    //sprite.cpp
    auto scanline(uint16 y) -> void;
    auto render(uint16 y) -> void;

    adaptive_array<Object, 16> objects;

    uint1 enable;

    struct Output {
      uint4 color;
      uint4 palette;
      uint1 priority;
    } output[512];
  } sprite;
};
