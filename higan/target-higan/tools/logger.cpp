LogSettings::LogSettings(View* parent) : Panel(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);

  logHeader.setText("Logger").setFont(Font().setBold());
  logList.setHeadered();
  logList.onToggle([&](auto cell) { eventToggle(cell); });
}

auto LogSettings::show() -> void {
  setVisible(true);
  refresh();
}

auto LogSettings::hide() -> void {
  setVisible(false);
}

auto LogSettings::refresh() -> void {
  logList.reset();
  logList.append(TableViewColumn());
  logList.append(TableViewColumn().setText("ID"));
  logList.append(TableViewColumn().setText("Name").setExpandable());
  for(auto& tracer : higan::logger.tracers) {
    TableViewItem item{&logList};
    item.append(TableViewCell().setCheckable().setChecked(tracer->enabled()));
    item.append(TableViewCell().setText(tracer->source()));
    item.append(TableViewCell().setText("tracer"));
    item.setProperty<shared_pointer<higan::Tracer>>("tracer", tracer);
  }
  for(auto& notification : higan::logger.notifications) {
    TableViewItem item{&logList};
    item.append(TableViewCell().setCheckable().setChecked(notification->enabled()));
    item.append(TableViewCell().setText(notification->source()));
    item.append(TableViewCell().setText(notification->name()));
    item.setProperty<shared_pointer<higan::Notification>>("notification", notification);
  }
  logList.resizeColumns();
}

auto LogSettings::eventToggle(TableViewCell cell) -> void {
  if(auto item = cell->parentTableViewItem()) {
    if(auto tracer = item->property<shared_pointer<higan::Tracer>>("tracer")) {
      tracer->setEnabled(cell.checked());
    }
    if(auto notification = item->property<shared_pointer<higan::Notification>>("notification")) {
      notification->setEnabled(cell.checked());
    }
  }
}
