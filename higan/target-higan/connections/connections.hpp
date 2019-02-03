struct ConnectionListDialog : Window {
  ConnectionListDialog();
  auto select(higan::Interface::Edge) -> higan::Interface::Node;
  auto eventAccept() -> void;
  auto eventListChange() -> void;

  VerticalLayout layout{this};
    TableView nodeList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  higan::Interface::Edge edge;
  higan::Interface::Node node;
};

struct ConnectionManager : Window {
  ConnectionManager();
  auto show(Window parent) -> void;
  auto refresh() -> void;
  template<typename T> auto attach(T parent, higan::Interface::Node) -> void;
  auto eventActivate() -> void;

  VerticalLayout layout{this};
    TreeView connectionList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button configureButton{&controlLayout, Size{100, 0}};

  ConnectionListDialog listDialog;
};

namespace Instances { extern Instance<ConnectionManager> connectionManager; }
extern ConnectionManager& connectionManager;
