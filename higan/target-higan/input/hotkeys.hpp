struct InputHotkey {
  InputHotkey(string_view name);
  auto poll() -> void;

  const string name;

  shared_pointer<HID::Device> device;
  uint pathID = 0;
  uint vendorID = 0;
  uint productID = 0;
  uint groupID = 0;
  uint inputID = 0;

  bool oldValue = 0;
  bool newValue = 0;

  function<void ()> onPress;
  function<void ()> onRelease;
};

struct Hotkeys {
  Hotkeys();
  auto poll() -> void;
  auto bind() -> void;

  vector<InputHotkey> hotkeys;

private:
  uint stateSlot = 1;
};
