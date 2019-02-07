ConfigurationReviewPanel::ConfigurationReviewPanel(HorizontalLayout* parent, Size size) : VerticalLayout(parent, size) {
  nodeList.setBackgroundColor(Theme::BackgroundColor);
  nodeList.setForegroundColor(Theme::ForegroundColor);
  launchButton.setText("Launch").onActivate([&] { eventLaunch(); });
}

auto ConfigurationReviewPanel::refresh(string location) -> void {
  nodeList.reset();
  auto identity = BML::unserialize(file::read({location, "identity.bml"}));
  auto root = higan::Node::unserialize(file::read({location, "node.bml"}));
  nodeList.append(TreeViewItem().setText(identity["system"].text()));
  //bit of a hack reusing SystemManager::attach() here, but it saves code duplication, so ...
  for(auto& node : *root) systemManager.attach(nodeList, node);
  nodeList.expand();
}

auto ConfigurationReviewPanel::eventLaunch() -> void {
  if(auto item = configurationManager.configurationList.selected()) {
    if(auto system = item.property("system")) {
      if(auto index = interfaces.find([&](auto interface) { return interface->name() == system; })) {
        configurationManager.setVisible(false);
        emulator.create(interfaces[*index], item.property("location"));
      }
    }
  }
}
