InputMapper::InputMapper(View* view) : Panel(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  nameLabel.setFont(Font().setBold());
  inputList.setBatchable();
  inputList.onActivate([&] { eventAssign(); });
  inputList.onChange([&] { eventChange(); });
  message.setFont(Font().setBold());
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
  message.setText();
}

auto InputMapper::update() -> void {
  for(auto& item : inputList.items()) {
    auto value = item.cell(1);
    auto node = item.property<higan::Node::Input>("node");
    if(auto name = node->property("inputName")) {
      value.setText(name).setFont();
    } else {
      value.setText("(unmapped)").setFont(Font().setSize(7.0));
    }
  }

  inputList.resizeColumns();
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
    inputList.setFocused();
    for(auto& item : inputList.batched()) item.setSelected(false);
    message.setText();
    return eventChange();
  }

  //use the viewport to sink inputs away from the table view during assignment
  programWindow.viewport.setFocused();
  auto item = assigningQueue.takeFirst();
  item.setFocused();  //scroll the current assigning mapping into view
  auto input = item.property<higan::Node::Input>("node");
  message.setText({"Assign: ", input->name});
  assigning = input;
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
    input->setProperty("groupName");
    input->setProperty("inputName");
  }
  inputManager.bind();
  update();
}

auto InputMapper::eventChange() -> void {
  auto batched = inputList.batched();
  assignButton.setText((bool)batched ? "Assign" : "Assign All");
  clearButton.setText((bool)batched ? "Clear" : "Clear All");
}

auto InputMapper::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(!assigning || !device->isKeyboard() || oldValue == 1 || newValue == 0) return;
  assigning->setProperty("pathID", device->pathID());
  assigning->setProperty("vendorID", device->vendorID());
  assigning->setProperty("productID", device->productID());
  assigning->setProperty("groupID", group);
  assigning->setProperty("inputID", input);
  assigning->setProperty("groupName", device->group(group).name());
  assigning->setProperty("inputName", device->group(group).input(input).name());
  assigning.reset();
  inputManager.bind();
  update();
  eventAssignNext();
}
