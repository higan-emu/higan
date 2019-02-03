struct InputMappingDialog : Window {
  InputMappingDialog();
  auto map(higan::Node) -> void;
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

  higan::Node node;
  higan::Node::Input assigning;
};

struct PortConfigurationDialog : Window {
  PortConfigurationDialog();
  auto configure(higan::Node::Port) -> bool;

  VerticalLayout layout{this};
    TableLayout tableLayout{&layout, Size{~0, 0}};
      Label pathObjectsLabel{&tableLayout, Size{0, 0}};
      LineEdit pathObjectsValue{&tableLayout, Size{~0, 0}};
      Button pathObjectsSelect{&tableLayout, Size{80, 0}};
      Button pathObjectsClear{&tableLayout, Size{80, 0}};
      Label pathTemplatesLabel{&tableLayout, Size{0, 0}};
      LineEdit pathTemplatesValue{&tableLayout, Size{~0, 0}};
      Button pathTemplatesSelect{&tableLayout, Size{80, 0}};
      Button pathTemplatesClear{&tableLayout, Size{80, 0}};

  higan::Node::Port port;
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

  higan::Node root;
  higan::Node::Port port;
};

struct SystemManager : Window {
  SystemManager();
  auto show() -> void;
  auto refresh() -> void;
  auto selected() -> higan::Node;
  template<typename T> auto attach(T parent, higan::Node) -> void;
  auto eventChange() -> void;
  auto eventActivate() -> void;
  auto eventConfigure() -> void;
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
      Button configureButton{&controlLayout, Size{100, 0}};

  StatusBar statusBar{this};

  InputMappingDialog inputMapping;
  PortConfigurationDialog portConfiguration;
  PortSelectionDialog portSelection;

  higan::Node root;
};

namespace Instances { extern Instance<SystemManager> systemManager; }
extern SystemManager& systemManager;
