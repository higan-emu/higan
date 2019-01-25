struct Properties : Setting<> {
  struct Memory : Setting<> { using Setting::Setting;
    Setting<string> type{this, "type", "EEPROM"};
    Setting<natural> size{this, "size"};
    Setting<string> content{this, "content", "Save"};
  } memory{this, "memory"};

  Properties() : Setting{"system"} {
  }
};

extern Properties propertyWonderSwan;
extern Properties propertyWonderSwanColor;
extern Properties propertyPocketChallengeV2;
