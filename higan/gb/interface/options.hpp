struct Options : Setting<> {
  struct Video : Setting<> { using Setting::Setting;
    Setting<boolean> blurEmulation{this, "blurEmulation", true};
    Setting<boolean> colorEmulation{this, "colorEmulation", true};
  } video{this, "video"};

  Options() : Setting{"options"} {
  }
};

extern Options option;
