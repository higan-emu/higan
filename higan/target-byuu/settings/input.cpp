InputSettings::InputSettings() {
  setCollapsible();
  setVisible(false);

  inputList.setHeadered();

  reload();

  assignButton.setText("Assign");
  clearButton.setText("Clear");
}

auto InputSettings::reload() -> void {
  inputList.reset();
  inputList.append(TableViewColumn().setText("Name"));
  inputList.append(TableViewColumn().setText("Mapping #1").setExpandable());
  inputList.append(TableViewColumn().setText("Mapping #2").setExpandable());
  inputList.append(TableViewColumn().setText("Mapping #3").setExpandable());

  vector<string> mappings{
    "D-Pad Up", "D-Pad Down", "D-Pad Left", "D-Pad Right",
    "Lower Left", "Lower Middle", "Lower Right",
    "Upper Left", "Upper Middle", "Upper Right",
    "Trigger Left", "Trigger Right",
    "Control Left", "Control Right"
  };
  for(auto& mapping : mappings) {
    TableViewItem item{&inputList};
    item.append(TableViewCell().setText(mapping).setFont(Font().setBold()));
    item.append(TableViewCell());
    item.append(TableViewCell());
    item.append(TableViewCell());
  }
}
