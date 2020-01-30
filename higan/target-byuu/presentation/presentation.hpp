struct Presentation : Window {
  Presentation();

  MenuBar menuBar{this};
    Menu loadMenu{&menuBar};
    Menu systemMenu{&menuBar};
    Menu settingsMenu{&menuBar};
      MenuItem configurationAction{&settingsMenu};
    Menu helpMenu{&menuBar};
      MenuItem webpageAction{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem aboutAction{&helpMenu};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};

  StatusBar statusBar{this};
};

namespace Instances { extern Instance<Presentation> presentation; }
extern Presentation& presentation;
