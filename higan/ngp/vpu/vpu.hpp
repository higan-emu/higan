//K1GE: K1 Graphics Engine (Neo Geo Pocket)
//K2GE: K2 Graphics Engine (Neo Geo Pocket Color)

struct VPU : Thread {
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
  auto readSprite(uint8 address) -> uint8;
  auto writeSprite(uint8 address, uint8 data) -> void;
  auto readSpriteColor(uint6 address) -> uint8;
  auto writeSpriteColor(uint6 address, uint8 data) -> void;
  auto readPaletteColor(uint9 address) -> uint8;
  auto writePaletteColor(uint9 address, uint8 data) -> void;

  //window.cpp
  auto renderWindow() -> maybe<uint12>;

  //plane.cpp
  struct Plane;
  auto renderPlane(Plane&) -> maybe<uint12>;

  //sprite.cpp
  auto renderSprite() -> maybe<uint12>;

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  uint32 buffer[160 * 152];
  uint12 colorPalette[256];

  struct Background {
    uint3 color;
    uint3 unused;
    uint2 mode;  //2 = on; 0,1,3 = off (black)
  } background;

  struct Window {
    uint8 hoffset;
    uint8 voffset;
    uint8 hlength;
    uint8 vlength;
    uint3 color;  //color drawn outside of window
  } window;

  struct Plane {
    uint12 address;
    uint8  colorNative;  //K2GE
    uint8  colorCompatible;  //K2GE
    uint8  hscroll;
    uint8  vscroll;
    uint3  palette[2][4];
  } plane1, plane2;

  struct Sprites {
    uint8 colorNative;  //K2GE
    uint8 colorCompatible;  //K2GE
    uint8 hscroll;
    uint8 vscroll;
    uint3 palette[2][4];
  } sprite;

  struct Sprite {
    uint9 character;
    uint1 vchain;
    uint1 hchain;
    uint2 priority;
    uint1 palette;
    uint1 vflip;
    uint1 hflip;
    uint8 hoffset;
    uint8 voffset;
    uint4 code;  //K2GE

    uint8 x;
    uint8 y;
  } sprites[64];

  struct LED {
    uint8 control = 0x07;
    uint8 frequency = 0x80;
  } led;

  struct Screen {
    uint1 negate;     //0 = normal, 1 = switched display (not emulated)
    uint1 colorMode;  //0 = K2GE, 1 = K1GE compatible
  } screen;

  struct IO {
    uint8  vlines = 0xc6;
    uint8  vcounter;
    uint10 hcounter;

    uint1 hblankEnableIRQ = 1;  //todo: should be 0
    uint1 vblankEnableIRQ = 1;
    uint1 hblankActive;
    uint1 vblankActive;

    uint1 characterOver;

    uint1 planePriority;  //0 = plane1 > plane2; 1 = plane2 > plane1
  } io;
};

extern VPU vpu;
