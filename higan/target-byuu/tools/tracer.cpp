auto TraceLogger::construct() -> void {
  setCollapsible();
  setVisible(false);

  tracerLabel.setText("Tracer").setFont(Font().setBold());
  tracerList.onToggle([&](auto item) { eventToggle(item); });
}

auto TraceLogger::reload() -> void {
  tracerList.reset();
  for(auto event : higan::Node::enumerate<higan::Node::Event>(emulator->root)) {
    ListViewItem item{&tracerList};
    item.setAttribute<higan::Node::Event>("event", event);
    item.setCheckable();
    item.setText({event->component(), " ", event->name()});
  }
}

auto TraceLogger::unload() -> void {
  tracerList.reset();
  if(fp) fp.close();
}

auto TraceLogger::eventToggle(ListViewItem item) -> void {
  if(auto event = item.attribute<higan::Node::Event>("event")) {
    bool enabled = event->enabled();
    event->setEnabled(!enabled);
  }
}
