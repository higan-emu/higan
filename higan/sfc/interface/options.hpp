struct Options : Setting<> {
  struct Port : Setting<> { using Setting::Setting;
    struct Controller1 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::Gamepad};
    } controller1{this, "controller1"};

    struct Controller2 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::Gamepad};
    } controller2{this, "controller2"};

    struct Expansion : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::None};
    } expansion{this, "expansion"};
  } port{this, "port"};

  Setting<boolean> random{this, "entropy", true};

  struct Video : Setting<> { using Setting::Setting;
    Setting<boolean> blurEmulation{this, "blurEmulation", true};
    Setting<boolean> colorEmulation{this, "colorEmulation", true};
  } video{this, "video"};

  struct Hack : Setting<> { using Setting::Setting;
    struct PPU : Setting<> { using Setting::Setting;
      Setting<boolean> fast{this, "fast", false};
      Setting<boolean> noSpriteLimit{this, "noSpriteLimit", false};
      Setting<boolean> hiresMode7{this, "hiresMode7", false};
    } ppu{this, "ppu"};

    struct DSP : Setting<> { using Setting::Setting;
      Setting<boolean> fast{this, "fast", false};
    } dsp{this, "dsp"};

    struct Coprocessor : Setting<> { using Setting::Setting;
      Setting<boolean> fast{this, "fast", false};
    } coprocessor{this, "coprocessor"};
  } hack{this, "hack"};

  Options() : Setting{"options"} {
  }
};

extern Options option;
