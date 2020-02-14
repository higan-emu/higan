struct PPU : Thread, PPUcounter {
  Node::Component node;
  Node::Screen screen;
  Node::Boolean colorEmulation;

  inline auto interlace() const -> bool { return false; }
  inline auto overscan() const -> bool { return false; }
  inline auto vdisp() const -> uint { return 225; }

  //ppu.cpp
  auto load(Node::Object parent, Node::Object from) -> void;
  auto unload() -> void;

  auto step(uint clocks) -> void;
  auto main() -> void;
  auto map() -> void;
  auto power(bool reset) -> void;
  auto refresh() -> void;

  //io.cpp
  auto latchCounters() -> void;
  alwaysinline auto addressVRAM() const -> uint16;
  alwaysinline auto readVRAM() -> uint16;
  alwaysinline auto writeVRAM(uint1 byte, uint8 data) -> void;
  alwaysinline auto readOAM(uint10 address) -> uint8;
  alwaysinline auto writeOAM(uint10 address, uint8 data) -> void;
  alwaysinline auto readCGRAM(uint1 byte, uint8 address) -> uint8;
  alwaysinline auto writeCGRAM(uint8 address, uint15 data) -> void;
  auto readIO(uint24 address, uint8 data) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;
  auto updateVideoMode() -> void;

  //color.cpp
  auto color(uint32 color) -> uint64;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint32* output = nullptr;

  struct {
    uint4 version;
    uint8 mdr;
  } ppu1, ppu2;

  struct VRAM {
    inline auto& operator[](uint address) { return data[address & mask]; }
    uint16 data[64_KiB];
    uint16 mask = 0x7fff;
  } vram;

  struct Latches {
    uint16 vram;
     uint8 oam;
     uint8 cgram;
     uint8 bgofsPPU1;
     uint8 bgofsPPU2;
     uint8 mode7;
     uint1 counters;
     uint1 hcounter;
     uint1 vcounter;

    uint10 oamAddress;
     uint8 cgramAddress;
  } latch;

  struct IO {
    //$2100  INIDISP
    uint4 displayBrightness;
    uint1 displayDisable;

    //$2102  OAMADDL
    //$2103  OAMADDH
    uint10 oamBaseAddress;
    uint10 oamAddress;
     uint1 oamPriority;

    //$2105  BGMODE
     uint3 bgMode;
     uint1 bgPriority;

    //$210d  BG1HOFS
    uint16 hoffsetMode7;

    //$210e  BG1VOFS
    uint16 voffsetMode7;

    //$2115  VMAIN
     uint8 vramIncrementSize;
     uint2 vramMapping;
     uint1 vramIncrementMode;

    //$2116  VMADDL
    //$2117  VMADDH
    uint16 vramAddress;

    //$211a  M7SEL
     uint1 hflipMode7;
     uint1 vflipMode7;
     uint2 repeatMode7;

    //$211b  M7A
    uint16 m7a;

    //$211c  M7B
    uint16 m7b;

    //$211d  M7C
    uint16 m7c;

    //$211e  M7D
    uint16 m7d;

    //$211f  M7X
    uint16 m7x;

    //$2120  M7Y
    uint16 m7y;

    //$2121  CGADD
     uint8 cgramAddress;
     uint1 cgramAddressLatch;

    //$213c  OPHCT
    uint16 hcounter;

    //$213d  OPVCT
    uint16 vcounter;
  } io;

  #include "background.hpp"
  #include "oam.hpp"
  #include "object.hpp"
  #include "window.hpp"
  #include "dac.hpp"

  Background bg1{Background::ID::BG1};
  Background bg2{Background::ID::BG2};
  Background bg3{Background::ID::BG3};
  Background bg4{Background::ID::BG4};
  Object obj;
  Window window;
  DAC dac;

  friend class PPU::Background;
  friend class PPU::Object;
  friend class PPU::Window;
  friend class PPU::DAC;
  friend class SuperFamicomInterface;
};

extern PPU ppu;
