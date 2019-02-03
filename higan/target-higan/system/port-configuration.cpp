PortConfigurationDialog::PortConfigurationDialog() {
  layout.setPadding(5);
  tableLayout.setSize({4, 2});
  tableLayout.column(0).setAlignment(1.0);
  pathObjectsLabel.setText("Path:");
  pathObjectsValue.setEditable(false);
  pathObjectsSelect.setText("Select ...").onActivate([&] {
    if(auto location = BrowserDialog()
      .setTitle("Select Path")
      .setPath(Path::user())
      .setPlacement(Placement::Overlap, *this)
      .selectFolder()
    ) {
      port->setProperty("location", location);
      pathObjectsValue.setText(location);
    }
  });
  pathObjectsClear.setText("Clear").onActivate([&] {
    port->setProperty("location");
    pathObjectsValue.setText();
  });
  pathTemplatesLabel.setText("Templates:");
  pathTemplatesValue.setEditable(false);
  pathTemplatesSelect.setText("Select ...").onActivate([&] {
    if(auto location = BrowserDialog()
      .setTitle("Select Template Path")
      .setPath(Path::user())
      .setPlacement(Placement::Overlap, *this)
      .selectFolder()
    ) {
      port->setProperty("templates", location);
      pathTemplatesValue.setText(location);
    }
  });
  pathTemplatesClear.setText("Clear").onActivate([&] {
    port->setProperty("templates");
    pathTemplatesValue.setText();
  });

  onClose([&] {
    setVisible(false);
    setModal(false);
  });

  setDismissable();
}

auto PortConfigurationDialog::configure(higan::Node::Port port) -> bool {
  this->port = port;

  pathObjectsValue.setText(port->property("location"));
  pathTemplatesValue.setText(port->property("templates"));

  setTitle(port->name);
  setSize({480, layout.minimumSize().height()});
  setPlacement(Placement::Overlap, systemManager);
  setVisible();
  setFocused();
  setModal();
  return false;
}
