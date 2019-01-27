struct Properties : Setting<> {
  Setting<string> name{this, "name", "Game Boy Advance"};

  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "ROM"};
    Setting<natural> size{this, "size", 16384};
    Setting<string> content{this, "content", "BIOS"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties property;
