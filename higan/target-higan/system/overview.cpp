SystemOverviewPanel::SystemOverviewPanel(HorizontalLayout* parent, Size size) : VerticalLayout(parent, size) {
  treeView.setEnabled(false);
  spacerButton.setVisible(false);
}

auto SystemOverviewPanel::show() -> void {
  setVisible(true);
}

auto SystemOverviewPanel::hide() -> void {
  setVisible(false);
}
