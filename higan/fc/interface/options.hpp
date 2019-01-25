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

  struct Video : Setting<> { using Setting::Setting;
    Setting<boolean> colorEmulation{this, "colorEmulation", true};
  } video{this, "video"};

  Options() : Setting{"options"} {
    video.colorEmulation.onModify([&] {
      higan::video.setPalette();
    });
  }
};

extern Options option;
