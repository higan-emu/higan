struct Properties : Setting<string> {
  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "ROM"};
    Setting<natural> size{this, "size", 65536};
    Setting<string> content{this, "content", "BIOS"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties propertyNeoGeoPocket;
extern Properties propertyNeoGeoPocketColor;
