struct InputManager {
  Input input;

  auto create() -> void;
  auto reset() -> void;

  auto poll() -> void;
  auto bind() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

  higan::Node root;

private:
  vector<shared_pointer<HID::Device>> devices;

  uint64_t pollFrequency = 5;
  uint64_t lastPoll = 0;
};

namespace Instances { extern Instance<InputManager> inputManager; }
extern InputManager& inputManager;
