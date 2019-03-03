struct ActionMenu : Menu {
  ActionMenu(MenuBar*);
  MenuItem create{this};
  MenuItem rename{this};
  MenuItem remove{this};
  MenuSeparator quitSeparator{this};
  MenuItem quit{this};
};

struct SystemMenu : Menu {
  SystemMenu(MenuBar*);
  MenuCheckItem power{this};
};

struct SettingsMenu : Menu {
  SettingsMenu(MenuBar*);
  MenuItem video{this};
  MenuItem audio{this};
  MenuItem input{this};
  MenuItem hotkeys{this};
  MenuSeparator panelSeparator{this};
  MenuCheckItem showFrameRate{this};
  MenuCheckItem muteAudio{this};
  MenuSeparator showSeparator{this};
  MenuCheckItem showComponents{this};
  MenuCheckItem showHacks{this};
};

struct ToolsMenu : Menu {
  ToolsMenu(MenuBar*);
  Menu saveStateMenu{this};
    MenuItem saveState1{&saveStateMenu};
    MenuItem saveState2{&saveStateMenu};
    MenuItem saveState3{&saveStateMenu};
    MenuItem saveState4{&saveStateMenu};
    MenuItem saveState5{&saveStateMenu};
  Menu loadStateMenu{this};
    MenuItem loadState1{&loadStateMenu};
    MenuItem loadState2{&loadStateMenu};
    MenuItem loadState3{&loadStateMenu};
    MenuItem loadState4{&loadStateMenu};
    MenuItem loadState5{&loadStateMenu};
};

struct HelpMenu : Menu {
  HelpMenu(MenuBar*);
  MenuItem documentation{this};
  MenuSeparator separator{this};
  MenuItem about{this};
};
