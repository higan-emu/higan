struct ViewportWindow : Window {
  auto create() -> void;
  auto context() -> uintptr;
  auto show() -> void;

  MenuBar menuBar{this};
    Menu systemMenu{&menuBar};
      MenuItem connectionsAction{&systemMenu};
      MenuSeparator systemSeparator{&systemMenu};
      MenuCheckItem powerToggle{&systemMenu};
    Menu settingsMenu{&menuBar};
    Menu toolsMenu{&menuBar};
    Menu helpMenu{&menuBar};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};
};

namespace Instances { extern Instance<ViewportWindow> viewport; }
extern ViewportWindow& viewport;
