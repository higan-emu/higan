struct ConfigurationCreateDialog : Window {
  struct Result {
    string system;
    string name;
  };

  ConfigurationCreateDialog();
  auto run() -> Result;
  auto eventAccept() -> void;
  auto eventListChange() -> void;

  VerticalLayout layout{this};
    TableView systemList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label nameLabel{&controlLayout, Size{0, 0}};
      LineEdit nameValue{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  Result result;
};

struct ConfigurationPropertiesDialog : Window {
  ConfigurationPropertiesDialog();
  auto run(string name, string document) -> string;
  auto eventAccept() -> void;

  VerticalLayout layout{this};
    TextEdit propertiesDocument{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label systemLabel{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};

  string system;
  string result;
};

struct ConfigurationManager : Window {
  ConfigurationManager();
  auto show() -> void;
  auto refresh() -> void;
  template<typename T> auto scan(T parent, string location) -> void;
  auto eventActivate() -> void;
  auto eventChange() -> void;
  auto eventContext() -> void;
  auto eventCreate() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;
  auto eventProperties() -> void;

  MenuBar menuBar{this};
    Menu actionMenu{&menuBar};
      MenuItem launchAction{&actionMenu};
      MenuSeparator launchSeparator{&actionMenu};
      MenuItem createAction{&actionMenu};
      MenuItem renameAction{&actionMenu};
      MenuItem removeAction{&actionMenu};
      MenuSeparator actionSeparator{&actionMenu};
      MenuItem propertiesAction{&actionMenu};
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

  VerticalLayout layout{this};
    TreeView configurationList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Button createButton{&controlLayout, Size{80, 0}};
      Button renameButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};
      Button propertiesButton;  //{&controlLayout, Size{100, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button launchButton{&controlLayout, Size{80, 0}};

  ConfigurationCreateDialog createDialog;
  ConfigurationPropertiesDialog propertiesDialog;
};

namespace Instances { extern Instance<ConfigurationManager> configurationManager; }
extern ConfigurationManager& configurationManager;
