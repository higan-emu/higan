struct ConnectionDialog : Window {
  ConnectionDialog();
  auto connect(higan::Node::Node) -> string;
  auto eventAccept() -> void;
  auto eventChange() -> void;

  VerticalLayout layout{this};
    TableView nodeList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label nameLabel{&controlLayout, Size{0, 0}};
      LineEdit nameValue{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  higan::Node::Node node;
  string result;
};

struct ConnectionManager : Window {
  ConnectionManager();
  auto show() -> void;
  auto refresh() -> void;
  template<typename T> auto attach(T parent, higan::Node::Node) -> void;
  auto eventActivate() -> void;

  MenuBar menuBar{this};
    Menu systemMenu{&menuBar};
      MenuCheckItem powerToggle{&systemMenu};
    Menu settingsMenu{&menuBar};
    Menu toolsMenu{&menuBar};
    Menu helpMenu{&menuBar};
      MenuItem documentationAction{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem aboutAction{&helpMenu};

  VerticalLayout layout{this};
    TreeView connectionList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button configureButton{&controlLayout, Size{100, 0}};

  StatusBar statusBar{this};

  ConnectionDialog connectionDialog;
};

namespace Instances { extern Instance<ConnectionManager> connectionManager; }
extern ConnectionManager& connectionManager;
