struct ConnectionManager : Window {
  ConnectionManager();
  auto show(Window parent) -> void;
  auto refresh() -> void;
  template<typename T> auto attach(T parent, const higan::Interface::Object& object) -> void;

  VerticalLayout layout{this};
    TreeView connectionList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button configureButton{&controlLayout, Size{100, 0}};
};

namespace Instances { extern Instance<ConnectionManager> connectionManager; }
extern ConnectionManager& connectionManager;
