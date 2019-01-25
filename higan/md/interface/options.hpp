struct Options : Setting<> {
  struct Port : Setting<> { using Setting::Setting;
    struct Controller1 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::ControlPad};
    } controller1{this, "controller1"};

    struct Controller2 : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::ControlPad};
    } controller2{this, "controller2"};

    struct Extension : Setting<> { using Setting::Setting;
      Setting<natural> device{this, "device", ID::Device::None};
    } extension{this, "extension"};
  } port{this, "port"};

  Options() : Setting{"options"} {
  }
};

extern Options option;
