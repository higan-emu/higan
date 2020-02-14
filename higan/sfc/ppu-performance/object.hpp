struct Object {
  alwaysinline auto addressReset() -> void;
  alwaysinline auto setFirstSprite() -> void;

  OAM oam;

  struct IO {
     uint1 interlace;

    uint16 tiledataAddress;
     uint2 nameselect;
     uint3 baseSize;

     uint1 rangeOver;
     uint1 timeOver;
  } io;

  friend class PPU;
};
