SystemOverview::SystemOverview(View* parent) : PanelItem(parent, Size{~0, ~0}) {
  setCollapsible().setVisible(false);
  header.setFont(Font().setBold());
  removeButton.setText("Remove").onActivate([&] { systemManager.doRemove(); });
  renameButton.setText("Rename").onActivate([&] { systemManager.doRename(); });
  launchButton.setText("Launch").onActivate([&] { systemManager.onActivate(); });
}

auto SystemOverview::show() -> void {
  refresh();
  setVisible(true);
}

auto SystemOverview::hide() -> void {
  setVisible(false);
}

auto SystemOverview::refresh() -> void {
  auto location = systemManager.listView.selected().attribute("location");
  nodeList.reset();
  auto manifest = BML::unserialize(file::read({location, "manifest.bml"}));
  header.setText(manifest["system"].text());
  if(auto root = higan::Node::unserialize(file::read({location, "settings.bml"}))) {
    for(auto& node : *root) scan(node);
  }
}

auto SystemOverview::scan(higan::Node::Object node, uint depth) -> void {
  if(node->is<higan::Node::Input>()) return;
  if(node->is<higan::Node::Screen>()) return;
  if(node->is<higan::Node::Component>() && !settings.interface.advancedMode) return;

  ListViewItem item{&nodeList};
  string name;
  for(uint n : range(depth)) name.append("   ");
  name.append(node->attribute("name") ? node->attribute("name") : node->name());
  if(auto setting = node->cast<higan::Node::Setting>()) {
    name.append(": ", setting->readValue());
  }

  item.setText(name);
  for(auto& node : *node) scan(node, depth + 1);
}
