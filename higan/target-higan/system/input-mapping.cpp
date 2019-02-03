InputMappingDialog::InputMappingDialog() {
  layout.setPadding(5);
  inputList.setBackgroundColor(Theme::BackgroundColor);
  inputList.setForegroundColor(Theme::ForegroundColor);
  inputList.setHeadered();
  inputList.setBatchable();
  inputList.onActivate([&] {
    eventAssign();
  });
  inputList.onChange([&] {
    eventChange();
  });
  message.setFont(Font().setBold());
  assignButton.setText("Assign").onActivate([&] {
    eventAssign();
  });
  clearButton.setText("Clear").onActivate([&] {
    eventClear();
  });

  onClose([&] {
    assigning = {};
    setModal(false);
    setVisible(false);
  });

  setDismissable();
}

auto InputMappingDialog::map(higan::Node node) -> void {
  this->node = node;
  this->assigning = {};
  refresh();
  setTitle({node->property("name"), " (", node->name, ")"});
  setSize({480, 360});
  setPlacement(Placement::After, systemManager);
  setVisible();
  setFocused();
  setModal();
}

auto InputMappingDialog::refresh() -> void {
  inputList.reset().setEnabled();
  inputList.append(TableViewColumn().setText("Name"));
  inputList.append(TableViewColumn().setText("Mapping").setExpandable());
  for(auto& node : this->node->find<higan::Node::Input>()) {
    TableViewItem item{&inputList};
    item.setProperty("node", string::from(node));
    TableViewCell name{&item};
    name.setText(node->name).setFont(Font().setBold());
    TableViewCell value{&item};
    if(auto name = node->property("name")) {
      value.setText(name);
    } else {
      value.setText("(unmapped)").setForegroundColor({128, 128, 128});
    }
  }
  inputList.resizeColumns().doChange();
  message.setText();
}

auto InputMappingDialog::eventChange() -> void {
  auto batched = inputList.batched();
  assignButton.setEnabled(batched.size() == 1);
  clearButton.setEnabled((bool)batched);
}

auto InputMappingDialog::eventAssign() -> void {
  auto batched = inputList.batched();
  for(auto& item : batched) {
    auto input = item.property("node").to<higan::Node::Input>();
    message.setText({"Assign: ", input->name});
    inputList.setEnabled(false);
    assignButton.setEnabled(false);
    clearButton.setEnabled(false);
    assigning = input;
  }
}

auto InputMappingDialog::eventClear() -> void {
}

auto InputMappingDialog::eventInput(shared_pointer<HID::Device> device, uint group, uint input, int16_t oldValue, int16_t newValue) -> void {
  if(!assigning || !device->isKeyboard()) return;
  auto batched = inputList.batched();
  if(batched.size() != 1) return;
  auto item = batched.first();
  auto node = item.property("node").to<higan::Node::Input>();
  node->setProperty("name", device->group(group).input(input).name());
  node->setProperty("device", string::from(device));
  node->setProperty("group", group);
  node->setProperty("input", input);
  refresh();
}
