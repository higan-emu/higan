struct Options : Setting<> {
  struct Port : Setting<> { using Setting::Setting;
    struct Controller : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::Gamepad};
    } controller{this, "controller"};
  } port{this, "port"};

  Options() : Setting{"options"} {
  }
};

extern Options option;
