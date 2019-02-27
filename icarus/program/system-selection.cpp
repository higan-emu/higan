SystemSelection::SystemSelection(View* view) : Panel(view, Size{140, ~0}) {
  setCollapsible().setVisible(false);

  systemList.append(ListViewItem().setText("BS Memory"));
  systemList.append(ListViewItem().setText("ColecoVision"));
  systemList.append(ListViewItem().setText("Famicom"));
  systemList.append(ListViewItem().setText("Game Boy"));
  systemList.append(ListViewItem().setText("Game Boy Advance"));
  systemList.append(ListViewItem().setText("Game Boy Color"));
  systemList.append(ListViewItem().setText("Game Gear"));
  systemList.append(ListViewItem().setText("Master System"));
  systemList.append(ListViewItem().setText("Mega Drive"));
  systemList.append(ListViewItem().setText("MSX"));
  systemList.append(ListViewItem().setText("Neo Geo Pocket"));
  systemList.append(ListViewItem().setText("Neo Geo Pocket Color"));
  systemList.append(ListViewItem().setText("PC Engine"));
  systemList.append(ListViewItem().setText("Pocket Challenge V2"));
  systemList.append(ListViewItem().setText("SC-3000"));
  systemList.append(ListViewItem().setText("SG-1000"));
  systemList.append(ListViewItem().setText("Sufami Turbo"));
  systemList.append(ListViewItem().setText("Super Famicom"));
  systemList.append(ListViewItem().setText("SuperGrafx"));
  systemList.append(ListViewItem().setText("WonderSwan"));
  systemList.append(ListViewItem().setText("WonderSwan Color"));
  systemList.onChange([&] { eventChange(); });
}

auto SystemSelection::eventChange() -> void {
  if(auto item = systemList.selected()) {
    gameManager.select(item.text());
  } else {
    programWindow.show(home);
  }
}
