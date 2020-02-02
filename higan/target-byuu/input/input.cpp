#include "../byuu.hpp"

VirtualPad virtualPad;
InputManager inputManager;

auto InputMapping::bind() -> void {
  device.reset();
  deviceID = 0;
  groupID = 0;
  inputID = 0;
  if(auto p = assignment.split("/"); p.size() == 4) {
    for(auto& device : inputManager.devices) {
      if(device->name() == p[0]) {
        if(device->id() == p[1].hex()) {
          if(auto groupID = device->find(p[2])) {
            if(auto inputID = device->group(*groupID).find(p[3])) {
              this->device = device;
              this->deviceID = device->id();
              this->groupID = *groupID;
              this->inputID = *inputID;
              break;
            }
          }
        }
      }
    }
  }
}

auto InputMapping::icon() -> image {
  if(assignment.beginsWith("Keyboard/")) return Icon::Device::Keyboard;
  return {};
}

auto InputMapping::text() -> string {
  string text = assignment;
  text.trimLeft("Keyboard/1/Button/");
  return text;
}

auto InputMapping::value() -> int16_t {
  if(device) return device->group(groupID).input(inputID).value();
  return 0;
}

auto InputMapping::resetAssignment() -> void {
  device.reset();
  deviceID = 0;
  groupID = 0;
  inputID = 0;
  assignment = "";
}

auto InputMapping::setAssignment(shared_pointer<HID::Device> device, uint groupID, uint inputID) -> void {
  this->device = device;
  this->deviceID = device->id();
  this->groupID = groupID;
  this->inputID = inputID;
  assignment = {device->name(), "/", hex(device->id()), "/", device->group(groupID).name(), "/", device->group(groupID).input(inputID).name()};
}

//

VirtualPad::VirtualPad() {
  mappings.append(&up);
  mappings.append(&down);
  mappings.append(&left);
  mappings.append(&right);
  mappings.append(&select);
  mappings.append(&start);
  mappings.append(&a);
  mappings.append(&b);
  mappings.append(&c);
  mappings.append(&x);
  mappings.append(&y);
  mappings.append(&z);
  mappings.append(&l);
  mappings.append(&r);
}

//

auto InputManager::bind() -> void {
  for(auto& mapping : virtualPad.mappings) mapping->bind();
}

auto InputManager::poll() -> void {
  //polling actual hardware is very time-consuming; skip call if poll was called too recently
  auto thisPoll = chrono::millisecond();
  if(thisPoll - lastPoll < pollFrequency) return;
  lastPoll = thisPoll;

  auto devices = ruby::input.poll();
  bool changed = devices.size() != this->devices.size();
  if(!changed) {
    for(uint index : range(devices.size())) {
      changed = devices[index] != this->devices[index];
      if(changed) break;
    }
  }
  if(changed) {
    this->devices = devices;
    bind();
  }
}

auto InputManager::eventInput(shared_pointer<HID::Device> device, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void {
  inputSettings.eventInput(device, groupID, inputID, oldValue, newValue);
  hotkeySettings.eventInput(device, groupID, inputID, oldValue, newValue);
}
