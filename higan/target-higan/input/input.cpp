#include "../higan.hpp"

InputManager inputManager;

auto InputManager::create() -> void {
  input.create("SDL");
  input.setContext(systemManager.handle());
  input.onChange({&InputManager::eventInput, this});
}

auto InputManager::reset() -> void {
  input.reset();
}

auto InputManager::poll() -> void {
  //polling actual hardware is very time-consuming: skip call if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < pollFrequency) return;
  lastPoll = thisPoll;

  devices = input.poll();
}

auto InputManager::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  systemManager.eventInput(device, group, input, oldValue, newValue);
}
