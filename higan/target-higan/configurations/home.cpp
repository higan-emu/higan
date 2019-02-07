ConfigurationHomePanel::ConfigurationHomePanel(HorizontalLayout* parent, Size size) : VerticalLayout(parent, size) {
  image icon{Resource::Icon};
  icon.shrink();
//icon.alphaBlend(0xfffff0);
  iconCanvas.setIcon(icon);
  iconLayout.cell(iconCanvas).setSize({icon.width(), icon.height()});
  spacerButton.setVisible(false);
  setGeometry(geometry());
}
