InputMapper::InputMapper(View* view) : Panel(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  nameLabel.setFont(Font().setBold());
  inputList.setBatchable();
  inputList.onActivate([&] { eventAssign(); });
  inputList.onChange([&] { eventChange(); });
  mouseXaxis.setText("Mouse X-axis").setCollapsible().onActivate([&] { eventAssignMouse(0, 0); });
  mouseYaxis.setText("Mouse Y-axis").setCollapsible().onActivate([&] { eventAssignMouse(0, 1); });
  mouseLeft.setText("Mouse Left").setCollapsible().onActivate([&] { eventAssignMouse(1, 0); });
  mouseMiddle.setText("Mouse Middle").setCollapsible().onActivate([&] { eventAssignMouse(1, 1); });
  mouseRight.setText("Mouse Right").setCollapsible().onActivate([&] { eventAssignMouse(1, 2); });
  assignButton.setText("Assign").onActivate([&] { eventAssign(); });
  clearButton.setText("Clear").onActivate([&] { eventClear(); });
}

auto InputMapper::show() -> void {
  setVisible(true);
}

auto InputMapper::hide() -> void {
  setVisible(false);
  node = {};
  inputList.reset();
}

auto InputMapper::refresh(higan::Node::Object node) -> void {
  this->node = node;

  nameLabel.setText(node->name);
  inputList.reset().setEnabled();
  inputList.append(TableViewColumn().setText("Name").setBackgroundColor({240, 240, 255}));
  inputList.append(TableViewColumn().setText("Mapping").setExpandable());
  for(auto& node : node->find<higan::Node::Input>()) {
    if(node->parent != this->node) continue;
    TableViewItem item{&inputList};
    item.setProperty<higan::Node::Input>("node", node);
    TableViewCell name{&item};
    name.setText(node->name).setFont(Font().setBold());
    TableViewCell value{&item};
  }
  update();
  inputList.doChange();
}

auto InputMapper::update() -> void {
  for(auto& item : inputList.items()) {
    auto value = item.cell(1);
    auto node = item.property<higan::Node::Input>("node");
    if(auto name = node->property("inputName")) {
      string label{node->property("deviceName")};
      if(label == "Joypad") label.append(".", node->property("groupName"));
      label.append(".", node->property("inputName"));
      if(auto qualifier = node->property("qualifier")) label.append(".", qualifier);
      value.setText(label).setFont();
    } else {
      value.setText("(unmapped)").setFont(Font().setSize(7.0));
    }
  }

  inputList.resizeColumns();
}

auto InputMapper::eventAssignMouse(uint groupID, uint inputID) -> void {
  auto batched = inputList.batched();
  if(batched.size() != 1) return;
  for(auto& device : inputManager.devices) {
    if(!device->isMouse()) continue;
    assigning = batched.first().property<higan::Node::Input>("node");
    eventInput(device, groupID, inputID, 0, 1, true);
    return;
  }
}

auto InputMapper::eventAssign() -> void {
  //prevent an activation via the return key from mapping return instantly
  inputManager.poll();

  assigningQueue = inputList.batched();  //assign selected
  if(!assigningQueue) assigningQueue = inputList.items();  //assign all
  if(!assigningQueue) return;  //no inputs to assign

  eventAssignNext();
}

auto InputMapper::eventAssignNext() -> void {
  if(!assigningQueue) {
    inputList.selectNone().setFocused();
    return eventChange();
  }

  //use the viewport to sink inputs away from the table view during assignment
  programWindow.viewport.setFocused();
  auto item = assigningQueue.takeFirst();
  inputList.selectNone();
  item.setSelected().setFocused();  //scroll the current assigning mapping into view
  auto input = item.property<higan::Node::Input>("node");
  item.cell(1).setText("(assign)");
  assigning = input;
  eventChange();
}

auto InputMapper::eventClear() -> void {
  auto batched = inputList.batched();  //clear selected
  if(!batched) batched = inputList.items();  //clear all
  if(!batched) return;  //no inputs to clear

  for(auto& item : batched) {
    auto input = item.property<higan::Node::Input>("node");
    input->setProperty("pathID");
    input->setProperty("vendorID");
    input->setProperty("productID");
    input->setProperty("groupID");
    input->setProperty("inputID");
    input->setProperty("deviceName");
    input->setProperty("groupName");
    input->setProperty("inputName");
    input->setProperty("qualifier");
  }
  inputManager.bind();
  update();
}

auto InputMapper::eventChange() -> void {
  auto batched = inputList.batched();
  bool showMouseAxes = false;
  bool showMouseButtons = false;
  if(batched.size() == 1 && assigning) {
    if(auto node = batched.first().property<higan::Node::Input>("node")) {
      if(node->cast<higan::Node::Axis>()) showMouseAxes = true;
      if(node->cast<higan::Node::Button>()) showMouseButtons = true;
    }
  }
  mouseXaxis.setVisible(showMouseAxes);
  mouseYaxis.setVisible(showMouseAxes);
  mouseLeft.setVisible(showMouseButtons);
  mouseMiddle.setVisible(showMouseButtons);
  mouseRight.setVisible(showMouseButtons);
  controlLayout.resize();
  assignButton.setText((bool)batched ? "Assign" : "Assign All");
  clearButton.setText((bool)batched ? "Clear" : "Clear All");
}

auto InputMapper::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue, bool allowMouseInput) -> void {
  if(!assigning) return;

  auto isButton  = (bool)assigning->cast<higan::Node::Button>();
  auto isAxis    = (bool)assigning->cast<higan::Node::Axis>();
  auto isTrigger = (bool)assigning->cast<higan::Node::Trigger>();
  auto isRumble  = (bool)assigning->cast<higan::Node::Rumble>();

  bool allow = false;
  string qualifier;

  if(isButton) {
    if(device->isKeyboard()) {
      if(oldValue == 0 && newValue == 1) allow = true;
    }
    if(device->isJoypad() && (group == HID::Joypad::Axis || group == HID::Joypad::Hat)) {
      if(newValue < -24576) allow = true, qualifier = "Lo";
      if(newValue > +24576) allow = true, qualifier = "Hi";
    }
    if(device->isJoypad() && group == HID::Joypad::Button) {
      if(oldValue == 0 && newValue == 1) allow = true;
    }
  }

  if(isAxis) {
  }

  if(isTrigger) {
  }

  if(isRumble) {
  }

  if(allowMouseInput && device->isMouse()) allow = true;
  if(!allow) return;

  if(device->group(group).input(input).name() == "Escape") {
    assigning->setProperty("pathID");
    assigning->setProperty("vendorID");
    assigning->setProperty("productID");
    assigning->setProperty("groupID");
    assigning->setProperty("inputID");
    assigning->setProperty("deviceName");
    assigning->setProperty("groupName");
    assigning->setProperty("inputName");
    assigning->setProperty("qualifier");
  } else {
    assigning->setProperty("pathID", device->pathID());
    assigning->setProperty("vendorID", device->vendorID());
    assigning->setProperty("productID", device->productID());
    assigning->setProperty("groupID", group);
    assigning->setProperty("inputID", input);
    assigning->setProperty("deviceName", device->name());
    assigning->setProperty("groupName", device->group(group).name());
    assigning->setProperty("inputName", device->group(group).input(input).name());
    assigning->setProperty("qualifier", qualifier);
  }
  assigning.reset();
  inputManager.bind();
  update();
  eventAssignNext();
}
