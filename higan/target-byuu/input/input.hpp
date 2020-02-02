struct InputMapping {
  InputMapping(const string& name) : name(name) {}

  auto bind() -> void;
  auto icon() -> image;
  auto text() -> string;
  auto value() -> int16_t;

  auto resetAssignment() -> void;
  auto setAssignment(shared_pointer<HID::Device>, uint groupID, uint inputID) -> void;

  const string name;
  shared_pointer<HID::Device> device;
  uint64_t deviceID;
  uint groupID;
  uint inputID;
  string assignment;
};

struct InputButton : InputMapping {
  using InputMapping::InputMapping;
};

struct VirtualPad {
  VirtualPad();

  InputButton up{"Up"}, down{"Down"}, left{"Left"}, right{"Right"};
  InputButton select{"Select"}, start{"Start"};
  InputButton a{"A"}, b{"B"}, c{"C"};
  InputButton x{"X"}, y{"Y"}, z{"Z"};
  InputButton l{"L"}, r{"R"};

  vector<InputMapping*> mappings;
};

struct InputManager {
  auto bind() -> void;
  auto poll() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void;

  vector<shared_pointer<HID::Device>> devices;

  uint64_t pollFrequency = 5;
  uint64_t lastPoll = 0;
};

extern VirtualPad virtualPad;
extern InputManager inputManager;
