struct Properties : Setting<> {
  struct CPU : Setting<> { using Setting::Setting;
    Setting<natural> version{this, "version", 0};
  } cpu{this, "cpu"};

  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "ROM"};
    Setting<natural> size{this, "size", 2048};
    Setting<string> content{this, "content", "Boot"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties property;
