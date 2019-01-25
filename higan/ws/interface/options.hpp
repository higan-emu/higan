struct Options : Setting<> {
  struct Video : Setting<> { using Setting::Setting;
    Setting<boolean> interframeBlending{this, "interframeBlending", true};
    Setting<boolean> colorEmulation{this, "colorEmulation", true};
    Setting<boolean> rotateLeft{this, "rotateLeft", false};
  } video{this, "video"};

  Options() : Setting{"options"} {
    video.interframeBlending.onModify([&] {
      higan::video.setEffect(higan::Video::Effect::InterframeBlending, video.interframeBlending());
    });
    video.colorEmulation.onModify([&] {
      higan::video.setPalette();
    });
    video.rotateLeft.onModify([&] {
      higan::video.setEffect(higan::Video::Effect::RotateLeft, video.rotateLeft());
    });
  }
};

extern Options option;
