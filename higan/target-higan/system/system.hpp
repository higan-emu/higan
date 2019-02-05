struct InputMappingDialog : Window {
  InputMappingDialog();
  auto map(higan::Node::Object) -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventAssign() -> void;
  auto eventClear() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

  VerticalLayout layout{this};
    TableView inputList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label message{&controlLayout, Size{~0, 0}};
      Button assignButton{&controlLayout, Size{80, 0}};
      Button clearButton{&controlLayout, Size{80, 0}};

  higan::Node::Object node;
  higan::Node::Input assigning;
};

struct PortSelectionDialog : Window {
  PortSelectionDialog();
  auto select(higan::Node::Port) -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventAccept() -> void;

  VerticalLayout layout{this};
    TableView nodeList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label nameLabel{&controlLayout, Size{0, 0}};
      LineEdit nameValue{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  PopupMenu contextMenu;
    MenuItem renameAction{&contextMenu};
    MenuItem removeAction{&contextMenu};

  higan::Node::Object root;
  higan::Node::Port port;
};

struct SystemManager : Window {
  SystemManager();
  auto show() -> void;
  auto refresh() -> void;
  auto selected() -> higan::Node::Object;
  template<typename T> auto attach(T parent, higan::Node::Object) -> void;
  auto eventChange() -> void;
  auto eventActivate() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

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
      Button inputMappingButton{&controlLayout, Size{80, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};

  StatusBar statusBar{this};

  InputMappingDialog inputMapping;
  PortSelectionDialog portSelection;

  higan::Node::Object root;
};

namespace Instances { extern Instance<SystemManager> systemManager; }
extern SystemManager& systemManager;
