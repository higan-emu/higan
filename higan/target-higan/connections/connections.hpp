struct ConnectionDialog : Window {
  ConnectionDialog();
  auto connect(higan::Node) -> higan::Node;
  auto eventAccept() -> void;
  auto eventChange() -> void;

  VerticalLayout layout{this};
    TableView nodeList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label nameLabel{&controlLayout, Size{0, 0}};
      LineEdit nameValue{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  higan::Node node;
  higan::Node leaf;
};

struct ConnectionManager : Window {
  ConnectionManager();
  auto show(Window parent) -> void;
  auto refresh() -> void;
  template<typename T> auto attach(T parent, higan::Node) -> void;
  auto eventActivate() -> void;

  VerticalLayout layout{this};
    TreeView connectionList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button configureButton{&controlLayout, Size{100, 0}};

  ConnectionDialog connectionDialog;
};

namespace Instances { extern Instance<ConnectionManager> connectionManager; }
extern ConnectionManager& connectionManager;
