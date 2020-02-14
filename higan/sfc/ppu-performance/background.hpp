struct Background {
  struct ID { enum : uint { BG1, BG2, BG3, BG4 }; };
  const uint id;
  Background(uint id) : id(id) {}

  //background.cpp
  auto power() -> void;

  //mode7.cpp
  auto runMode7() -> void;

  struct IO {
     uint2 screenSize;
    uint16 screenAddress;

    uint16 tiledataAddress;
     uint1 tileSize;

    uint16 hoffset;
    uint16 voffset;
  } io;

  friend class PPU;
};
