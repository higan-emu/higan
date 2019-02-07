struct SystemOverviewPanel : VerticalLayout {
  SystemOverviewPanel(HorizontalLayout*, Size);
  auto show() -> void;
  auto hide() -> void;

  TreeView treeView{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Widget spacerWidget{&controlLayout, Size{~0, 0}};
    Button spacerButton{&controlLayout, Size{80, 0}};
};

struct InputMappingPanel : VerticalLayout {
  InputMappingPanel(HorizontalLayout*, Size);
  auto show(higan::Node::Object) -> void;
  auto hide() -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventAssign() -> void;
  auto eventClear() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint group, uint input, int16_t oldValue, int16_t newValue) -> void;

  TableView inputList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Label message{&controlLayout, Size{~0, 0}};
    Button assignButton{&controlLayout, Size{80, 0}};
    Button clearButton{&controlLayout, Size{80, 0}};

  higan::Node::Object node;
  higan::Node::Input assigning;
};

struct PortSelectionPanel : VerticalLayout {
  PortSelectionPanel(HorizontalLayout*, Size);
  auto show(higan::Node::Port) -> void;
  auto hide() -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventAccept() -> void;

  TableView nodeList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
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
  template<typename T> auto attach(T parent, higan::Node::Object, uint depth = 0) -> void;
  auto eventChange() -> void;
  auto eventChangePath() -> void;
  auto eventContext() -> void;
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

  PopupMenu contextMenu;
    MenuItem changePathAction{&contextMenu};

  HorizontalLayout layout{this};
    VerticalLayout connectionLayout{&layout, Size{250, ~0}, 0};
      TreeView connectionList{&connectionLayout, Size{~0, ~0}};
      HorizontalLayout controlLayout{&connectionLayout, Size{~0, 0}};
        Button spacerButton{&controlLayout, Size{80, 0}};
        Widget spacerWidget{&controlLayout, Size{~0, 0}};
    VerticalLayout resizeLayout{&layout, Size{9, ~0}, 0};
      HorizontalResizeGrip resizeGrip{&resizeLayout, Size{9, ~0}};
      Button resizeSpacer{&resizeLayout, Size{0, 0}};
    SystemOverviewPanel overview{&layout, Size{~0, ~0}};
    InputMappingPanel inputMapping{&layout, Size{~0, ~0}};
    PortSelectionPanel portSelection{&layout, Size{~0, ~0}};

  higan::Node::Object root;
};

namespace Instances { extern Instance<SystemManager> systemManager; }
extern SystemManager& systemManager;
