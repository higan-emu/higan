struct ConfigurationHomePanel : VerticalLayout {
  ConfigurationHomePanel(HorizontalLayout*, Size);

  Widget verticalSpacer{this, Size{~0, ~0}};
  HorizontalLayout iconLayout{this, Size{~0, 0}};
    Widget horizontalSpacer{&iconLayout, Size{~0, 0}};
    Canvas iconCanvas{&iconLayout, Size{0, 0}};
  HorizontalLayout controlLayout;  //{this, Size{~0, 0}};
    Widget spacerWidget{&controlLayout, Size{~0, 0}};
    Button spacerButton{&controlLayout, Size{80, 0}};
};

struct ConfigurationCreatePanel : VerticalLayout {
  struct Result {
    string system;
    string name;
  };

  ConfigurationCreatePanel(HorizontalLayout*, Size);
  auto show() -> void;
  auto eventAccept() -> void;
  auto eventChange() -> void;

  TableView systemList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Label nameLabel{&controlLayout, Size{0, 0}};
    LineEdit nameValue{&controlLayout, Size{~0, 0}};
    Button acceptButton{&controlLayout, Size{80, 0}};
};

struct ConfigurationReviewPanel : VerticalLayout {
  ConfigurationReviewPanel(HorizontalLayout*, Size);
  auto refresh(string location) -> void;
  auto eventLaunch() -> void;

  TreeView nodeList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Widget controlSpacer{&controlLayout, Size{~0, 0}};
    Button launchButton{&controlLayout, Size{80, 0}};
};

struct ConfigurationManager : Window {
  ConfigurationManager();
  auto show() -> void;
  auto refresh() -> void;
  template<typename T> auto scan(T parent, string location) -> void;
  template<typename T> auto attach(T parent, higan::Node::Object) -> void;
  auto eventChange() -> void;
  auto eventContext() -> void;
  auto eventCreate() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;

  MenuBar menuBar{this};
    Menu actionMenu{&menuBar};
      MenuItem launchAction{&actionMenu};
      MenuSeparator launchSeparator{&actionMenu};
      MenuItem createAction{&actionMenu};
      MenuItem renameAction{&actionMenu};
      MenuItem removeAction{&actionMenu};
      MenuSeparator quitSeparator{&actionMenu};
      MenuItem quitAction{&actionMenu};
    Menu settingsMenu{&menuBar};
    Menu pathsMenu{&menuBar};
      MenuItem dataPathChange{&pathsMenu};
      MenuItem dataPathOpen{&pathsMenu};
      MenuItem templatesPathOpen{&pathsMenu};
      MenuItem settingsPathOpen{&pathsMenu};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem about{&helpMenu};

  PopupMenu contextMenu;
    MenuItem launchPopup{&contextMenu};
    MenuSeparator launchPopupSeparator{&contextMenu};
    MenuItem createPopup{&contextMenu};
    MenuItem renamePopup{&contextMenu};
    MenuItem removePopup{&contextMenu};

  HorizontalLayout layout{this};
    VerticalLayout configurationLayout{&layout, Size{250, ~0}, 0};
      TreeView configurationList{&configurationLayout, Size{~0, ~0}};
      HorizontalLayout controlLayout{&configurationLayout, Size{~0, 0}};
        Button createButton{&controlLayout, Size{80, 0}};
        Button renameButton{&controlLayout, Size{80, 0}};
        Button removeButton{&controlLayout, Size{80, 0}};
    VerticalLayout resizeLayout{&layout, Size{9, ~0}, 0};
      HorizontalResizeGrip resizeGrip{&resizeLayout, Size{9, ~0}};
      Button resizeSpacer{&resizeLayout, Size{0, 0}};
    ConfigurationHomePanel homePanel{&layout, Size{~0, ~0}};
    ConfigurationCreatePanel createPanel{&layout, Size{~0, ~0}};
    ConfigurationReviewPanel reviewPanel{&layout, Size{~0, ~0}};
};

namespace Instances { extern Instance<ConfigurationManager> configurationManager; }
extern ConfigurationManager& configurationManager;
