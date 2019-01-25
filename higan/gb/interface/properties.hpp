struct Properties : Setting<string> {
  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "ROM"};
    Setting<natural> size{this, "size"};
    Setting<string> content{this, "content", "Boot"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties propertyGameBoy;
extern Properties propertyGameBoyColor;
