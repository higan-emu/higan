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
  inputList.append(TableViewColumn().setText("Name"));
  inputList.append(TableViewColumn().setText("Mapping").setExpandable());
  for(auto& node : node->find<higan::Node::Input>()) {
    if(node->parent != this->node) continue;
    TableViewItem item{&inputList};
    item.setProperty<higan::Node::Input>("node", node);
    TableViewCell name{&item};
    name.setText(node->name).setFont(Font().setBold());
    TableViewCell value{&item};
    if(auto name = node->property("name")) {
      value.setFont().setText(name);
    } else {
      value.setFont(Font().setSize(7.0)).setText("(unmapped)");
    }
  }
  inputList.resizeColumns().doChange();
  message.setText();
}

auto InputMapper::eventAssign() -> void {
  auto batched = inputList.batched();
  for(auto& item : batched) {
    auto input = item.property<higan::Node::Input>("node");
    message.setText({"Assign: ", input->name});
    inputList.setEnabled(false);
    assignButton.setEnabled(false);
    clearButton.setEnabled(false);
    assigning = input;
  }
}

auto InputMapper::eventClear() -> void {
  if(auto batched = inputList.batched()) {
    for(auto& item : batched) {
      auto input = item.property<higan::Node::Input>("node");
      input->setProperty("name");
      input->setProperty("pathID");
      input->setProperty("vendorID");
      input->setProperty("productID");
      input->setProperty("groupID");
      input->setProperty("inputID");
      inputManager.bind();
    }
    refresh(node);
  }
}

auto InputMapper::eventChange() -> void {
  auto batched = inputList.batched();
  assignButton.setEnabled(batched.size() == 1);
  clearButton.setEnabled((bool)batched);
}

auto InputMapper::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(!assigning || !device->isKeyboard()) return;
  assigning->setProperty("name", device->group(group).input(input).name());
  assigning->setProperty("pathID", device->pathID());
  assigning->setProperty("vendorID", device->vendorID());
  assigning->setProperty("productID", device->productID());
  assigning->setProperty("groupID", group);
  assigning->setProperty("inputID", input);
  assigning = {};
  inputManager.bind();
  refresh(node);
}
