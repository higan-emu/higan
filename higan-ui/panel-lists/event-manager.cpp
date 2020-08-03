EventManager::EventManager(View* view) : PanelList(view, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  listView.onChange([&] { onChange(); });
  listView.onToggle([&](auto item) { onToggle(item); });
}

auto EventManager::show() -> void {
  listView.selectNone().doChange();
  refresh();
  setVisible(true);
}

auto EventManager::hide() -> void {
  setVisible(false);
}

auto EventManager::refresh() -> void {
  listView.reset();
  if(auto root = emulator.root) {
    for(auto& node : *root) refresh(node);
  }
}

auto EventManager::refresh(higan::Node::Object node) -> void {
  if(auto tracer = node->cast<higan::Node::Tracer>()) {
    ListViewItem item{&listView};
    item.setAttribute<higan::Node::Tracer>("tracer", tracer);
    item.setCheckable();
    item.setChecked(tracer->enabled());
    item.setText({tracer->component(), " ", tracer->name()});
  }

  for(auto& node : *node) refresh(node);
}

auto EventManager::onChange() -> void {
}

auto EventManager::onToggle(ListViewItem item) -> void {
  if(auto tracer = item.attribute<higan::Node::Tracer>("tracer")) {
    tracer->setEnabled(item.checked());
  }
}
