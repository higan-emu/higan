struct Properties : Setting<> {
  struct CPU : Setting<> { using Setting::Setting;
    Setting<natural> version{this, "version", 2};
  } cpu{this, "cpu"};

  struct PPU1 : Setting<> { using Setting::Setting;
    Setting<natural> version{this, "version", 1};
    struct VRAM : Setting<> { using Setting::Setting;
      Setting<natural> size{this, "size", 65536};
    } vram{this, "vram"};
  } ppu1{this, "ppu1"};

  struct PPU2 : Setting<> { using Setting::Setting;
    Setting<natural> version{this, "version", 3};
  } ppu2{this, "ppu2"};

  Properties() : Setting{"system"} {
    cpu.version.setValid({1, 2});
    ppu1.version.setValid({1});
    ppu1.vram.size.setValid({65536, 131072});
    ppu2.version.setValid({1, 2, 3});
  }
};

extern Properties property;
