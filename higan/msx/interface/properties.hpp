struct Properties : Setting<> {
  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "ROM"};
    Setting<natural> size{this, "size", 32768};
    Setting<string> content{this, "content", "BIOS"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties property;
