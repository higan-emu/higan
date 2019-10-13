ActionMenu::ActionMenu(MenuBar* parent) : Menu(parent) {
  setText("System");
  create.setIcon(Icon::Action::Add).setText("Create").onActivate([&] {
    programWindow.setPanelList(systemManager);
    systemManager.doCreate();
  });
  rename.setIcon(Icon::Application::TextEditor).setText("Rename").setEnabled(false).onActivate([&] {
    systemManager.doRename();
  });
  remove.setIcon(Icon::Action::Remove).setText("Delete").setEnabled(false).onActivate([&] {
    systemManager.doRemove();
  });
  quit.setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] { emulator.quit(); });
}
