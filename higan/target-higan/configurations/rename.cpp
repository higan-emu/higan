ConfigurationRenameDialog::ConfigurationRenameDialog() {
  layout.setPadding(5);
  nameLabel.setText("Name:");
  nameValue.onActivate([&] {
    eventAccept();
  });
  acceptButton.setText("Rename").onActivate([&] {
    eventAccept();
  });

  onClose([&] {
    setModal(false);
    setVisible(false);
  });

  setDismissable();
  setSize({360, layout.minimumSize().height()});
}

auto ConfigurationRenameDialog::run(string name) -> string {
  result = {};
  nameValue.setText(name);
  setTitle({"Rename ", name});
  setPlacement(Placement::Overlap, configurationManager);
  setFocused();
  setVisible();
  nameValue.setFocused();
  setModal();
  return result;
}

auto ConfigurationRenameDialog::eventAccept() -> void {
  result = nameValue.text().strip();
  doClose();
}
