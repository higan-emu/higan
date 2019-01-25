struct Options : Setting<> {
  struct Port : Setting<> { using Setting::Setting;
    struct Controller1 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::Gamepad};
    } controller1{this, "controller1"};

    struct Controller2 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::Gamepad};
    } controller2{this, "controller2"};
  } port{this, "port"};

  Options() : Setting{"options"} {
  }
};

extern Options option;
