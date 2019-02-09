struct ActionMenu : Menu {
  ActionMenu(MenuBar*);
  MenuItem quit{this};
};

struct SystemMenu : Menu {
  SystemMenu(MenuBar*);
  MenuCheckItem power{this};
};

struct SettingsMenu : Menu {
  SettingsMenu(MenuBar*);
  MenuCheckItem showComponents{this};
  MenuCheckItem showHacks{this};
};

struct ToolsMenu : Menu {
  ToolsMenu(MenuBar*);
};

struct HelpMenu : Menu {
  HelpMenu(MenuBar*);
  MenuItem documentation{this};
  MenuSeparator separator{this};
  MenuItem about{this};
};
