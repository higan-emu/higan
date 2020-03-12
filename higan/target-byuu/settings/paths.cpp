auto PathSettings::construct() -> void {
  setCollapsible();
  setVisible(false);

  savesLabel.setText("Saves").setFont(Font().setBold());
  savesPath.setEditable(false);
  savesAssign.setText("Assign ...").onActivate([&] {
    BrowserDialog dialog;
    dialog.setTitle("Select Saves Path");
    dialog.setPath(Path::desktop());
    if(auto location = program.selectFolder(dialog)) {
      settings.paths.saves = location;
      refresh();
    }
  });
  savesReset.setText("Reset").onActivate([&] {
    settings.paths.saves = "";
    refresh();
  });

  patchesLabel.setText("BPS Patches").setFont(Font().setBold());
  patchesPath.setEditable(false);
  patchesAssign.setText("Assign ...").onActivate([&] {
    BrowserDialog dialog;
    dialog.setTitle("Select Patches Path");
    dialog.setPath(Path::desktop());
    if(auto location = program.selectFolder(dialog)) {
      settings.paths.patches = location;
      refresh();
    }
  });
  patchesReset.setText("Reset").onActivate([&] {
    settings.paths.patches = "";
    refresh();
  });

  firmwareLabel.setText("DSP Firmware").setFont(Font().setBold());
  firmwarePath.setEditable(false);
  firmwareAssign.setText("Assign ...").onActivate([&] {
    BrowserDialog dialog;
    dialog.setTitle("Select Firmware Path");
    dialog.setPath(Path::desktop());
    if(auto location = program.selectFolder(dialog)) {
      settings.paths.firmware = location;
      refresh();
    }
  });
  firmwareReset.setText("Reset").onActivate([&] {
    settings.paths.firmware = "";
    refresh();
  });

  refresh();
}

auto PathSettings::refresh() -> void {
  if(settings.paths.saves) {
    savesPath.setText(settings.paths.saves).setForegroundColor();
  } else {
    savesPath.setText("(same as game path)").setForegroundColor({80, 80, 80});
  }

  if(settings.paths.patches) {
    patchesPath.setText(settings.paths.patches).setForegroundColor();
  } else {
    patchesPath.setText("(same as game path)").setForegroundColor({80, 80, 80});
  }

  if(settings.paths.firmware) {
    firmwarePath.setText(settings.paths.firmware).setForegroundColor();
  } else {
    firmwarePath.setText("(same as game path)").setForegroundColor({80, 80, 80});
  }
}
