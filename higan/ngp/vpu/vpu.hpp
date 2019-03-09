//K1GE (Neo Geo Pocket)
//K2GE (Neo Geo Pocket Color)

struct VPU : Thread {
  Memory::Writable<uint8> spriteRAM;
  Memory::Writable<uint8> scrollRAM;
  Memory::Writable<uint8> characterRAM;

  //vpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto refresh() -> void;
  auto power() -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8;
  auto write(uint24 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  uint32 buffer[160 * 152];

  struct IO {
    uint8  vcounter;
    uint10 hcounter;

    uint1 hblankEnableIRQ = 1;  //todo: should be 0
    uint1 vblankEnableIRQ = 1;  //todo: should be 0
    uint1 hblankActive;
    uint1 vblankActive;

    uint3 outsideWindowColor;
    uint1 negateScreen;

    uint1 characterOver;

    uint1 planePriority;  //0 = plane1 > plane2; 1 = plane2 > plane1

    struct Plane {
      uint8 hscroll;
      uint8 vscroll;
      uint4 palette[2];
    } plane1, plane2;

    struct Sprite {
      uint8 hoffset;
      uint8 voffset;
      uint4 palette[2];
    } sprite;

    struct Window {
      uint8 horigin;
      uint8 vorigin;
      uint8 hsize;
      uint8 vsize;
    } window;

    struct LED {
      uint8 control = 0x07;
      uint8 frequency = 0x80;
    } led;
  } io;
};

extern VPU vpu;
