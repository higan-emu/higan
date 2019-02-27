SettingEditor::SettingEditor(View* view) : Panel(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  nameLabel.setFont(Font().setBold());
  latchedLayout.setCollapsible();
  latchedLabel.setText("Latched:");
  latchedValue;
  valueList.onChange([&] { eventChange(); });
}

auto SettingEditor::show() -> void {
  setVisible(true);
}

auto SettingEditor::hide() -> void {
  setVisible(false);
  setting = {};
}

auto SettingEditor::refresh(higan::Node::Setting setting) -> void {
  this->setting = setting;
  nameLabel.setText(setting->name);
  latchedLayout.setVisible(!setting->dynamic);
  latchedValue.setText(setting->readLatch());
  valueList.reset();
  for(auto& value : setting->readAllowedValues()) {
    ListViewItem item{&valueList};
    item.setText(value);
    if(value == setting->readValue()) item.setSelected();
  }
  resize();
}

auto SettingEditor::eventChange() -> void {
  if(auto item = valueList.selected()) {
    setting->writeValue(item.text());
    latchedValue.setText(setting->readLatch());
    nodeManager.refreshSettings();
  } else {
    //the current value should always be selected
    //if the user deselects all item, reselect the current value
    for(auto& item : valueList.items()) {
      if(item.text() == setting->readValue()) {
        item.setSelected();
        break;
      }
    }
  }
}
