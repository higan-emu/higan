auto EmulatorSettings::construct() -> void {
  setCollapsible();
  setVisible(false);

  emulatorLabel.setText("Load Menu Emulators:").setFont(Font().setBold());
  emulatorList.onToggle([&](auto cell) { eventToggle(cell); });
  emulatorList.append(TableViewColumn().setExpandable());
  for(auto& emulator : emulators) {
    TableViewItem item{&emulatorList};
    TableViewCell cell{&item};
    cell.setCheckable();
    cell.setChecked(emulator->configuration.visible);
    cell.setText(emulator->name);
  }
}

auto EmulatorSettings::eventToggle(TableViewCell cell) -> void {
  for(auto& emulator : emulators) {
    if(cell.text() == emulator->name) {
      emulator->configuration.visible = cell.checked();
      presentation.loadEmulators();  //update load menu emulator visibility
    }
  }
}
