struct LoadWindow : Window {
  LoadWindow();
  auto show() -> void;
  auto refresh() -> void;
  template<typename T> auto load(T parent, higan::Interface::Slot& slot) -> void;
  template<typename T> auto expand(T item) -> void;
  auto eventActivate() -> void;

  VerticalLayout layout{this};
    TreeView loadTree{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      ComboButton regionOption{&controlLayout, Size{0, 0}};
      Button bootButton{&controlLayout, Size{80, 0}};
};

struct Presentation : Window {
  enum : uint { StatusHeight = 24 };

  Presentation();
  auto refreshSystems() -> void;

  auto activate() -> void;
  auto deactivate() -> void;
  auto power(bool) -> void;

  auto load() -> void;
  auto unload() -> void;

  auto clearViewport() -> void;
  auto setStatusIcon(image icon) -> void;

  MenuBar menuBar{this};
    Menu systemsMenu{&menuBar};
    Menu systemMenu{&menuBar};
    Menu settingsMenu{&menuBar};
      MenuItem settingsSystems{&settingsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuSeparator helpSeparator{&helpMenu};
      MenuItem about{&helpMenu};

  VerticalLayout layout{this};
    HorizontalLayout horizontalLayout{&layout, Size{~0, ~0}, 0};
      Viewport viewport{&horizontalLayout, Size{~0, ~0}, 0};
      VerticalLayout verticalLayout{&horizontalLayout, Size{120, ~0}, 0};
        Widget programIconTop{&verticalLayout, Size{120, ~0}, 0};
        Canvas programIcon{&verticalLayout, Size{112, 112}, 0};
    HorizontalLayout statusLayout{&layout, Size{~0, StatusHeight}, 0};
      Label spacerIcon{&statusLayout, Size{8, ~0}, 0};
      Canvas statusIcon{&statusLayout, Size{16, ~0}, 0};
      Label spacerLeft{&statusLayout, Size{4, ~0}, 0};
      Label statusMessage{&statusLayout, Size{~0, ~0}, 0};
      Label statusMonitor{&statusLayout, Size{~0, ~0}, 0};
      Label spacerRight{&statusLayout, Size{8, ~0}, 0};

  MenuCheckItem powerStatus;
  LoadWindow loadWindow;
};

namespace Instances { extern Instance<Presentation> presentation; }
extern Presentation& presentation;
