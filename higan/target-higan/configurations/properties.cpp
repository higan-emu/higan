ConfigurationPropertiesDialog::ConfigurationPropertiesDialog() {
  layout.setPadding(5);
  propertiesDocument.setFont(Font().setFamily(Font::Mono));
  propertiesDocument.setBackgroundColor(Theme::BackgroundColor);
  propertiesDocument.setForegroundColor(Theme::ForegroundColor);
  systemLabel.setFont(Font().setBold());
  acceptButton.setText("Save").onActivate([&] {
    eventAccept();
  });

  onClose([&] {
    setModal(false);
    setVisible(false);
  });

  setDismissable();
  setSize({360, 325});
}

auto ConfigurationPropertiesDialog::run(string name, string markup) -> string {
  result = {};
  auto document = BML::unserialize(markup);
  system = document["system/name"].text();
  propertiesDocument.setText(markup);
  systemLabel.setText(system);
  setTitle({name, " Properties"});
  setCentered(configurationManager);
  setVisible();
  setFocused();
  propertiesDocument.setFocused();
  setModal();
  return result;
}

auto ConfigurationPropertiesDialog::eventAccept() -> void {
  auto markup = propertiesDocument.text();
  auto document = BML::unserialize(markup);
  if(!document) {
    if(MessageDialog()
      .setTitle("Error")
      .setText("Syntax error encountered parsing these properties.\n"
               "Continue trying to edit the properties to fix the error(s)?")
      .question() == "No"
    ) return doClose();
    return;
  }
  if(document["system/name"].text() != system) {
    if(MessageDialog()
      .setTitle("Error")
      .setText("The system name field has been changed, but this is not allowed.\n"
               "Instead, try removing this system and then creating a new one.\n"
               "Continue trying to edit the properties to fix the error(s)?")
      .question() == "No"
    ) return doClose();
    return;
  }
  result = markup;
  doClose();
}
