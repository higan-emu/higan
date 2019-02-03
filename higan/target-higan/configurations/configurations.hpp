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

struct ConfigurationRenameDialog : Window {
  ConfigurationRenameDialog();
  auto run(string) -> string;
  auto eventAccept() -> void;

  HorizontalLayout layout{this};
    Label nameLabel{&layout, Size{0, 0}};
    LineEdit nameValue{&layout, Size{~0, 0}};
    Button acceptButton{&layout, Size{80, 0}};

  string result;
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
  auto eventListChange() -> void;
  auto eventCreate() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;
  auto eventConnections() -> void;
  auto eventProperties() -> void;

  MenuBar menuBar{this};
    Menu actionMenu{&menuBar};
      MenuItem createAction{&actionMenu};
      MenuItem renameAction{&actionMenu};
      MenuItem removeAction{&actionMenu};
      MenuSeparator actionSeparator{&actionMenu};
      MenuItem connectionsAction{&actionMenu};
      MenuItem propertiesAction{&actionMenu};
      MenuSeparator quitSeparator{&actionMenu};
      MenuItem quitAction{&actionMenu};
    Menu settingsMenu{&menuBar};
    Menu helpMenu{&menuBar};
      MenuItem documentation{&helpMenu};
      MenuSeparator aboutSeparator{&helpMenu};
      MenuItem about{&helpMenu};

  VerticalLayout layout{this};
    HorizontalLayout horizontalLayout{&layout, Size{~0, ~0}};
      TreeView configurationList{&horizontalLayout, Size{~0, ~0}};
      VerticalLayout controlLayout{&horizontalLayout, Size{0, ~0}};
        Button createButton{&controlLayout, Size{90, 0}};
        Button renameButton{&controlLayout, Size{90, 0}};
        Button removeButton{&controlLayout, Size{90, 0}};
        Canvas controlSpacer{&controlLayout, Size{70, 1}};
        Button connectionsButton{&controlLayout, Size{90, 0}};
        Button propertiesButton{&controlLayout, Size{90, 0}};
    Label locationLabel{&layout, Size{0, 0}};

  ConfigurationCreateDialog createDialog;
  ConfigurationRenameDialog renameDialog;
  ConfigurationPropertiesDialog propertiesDialog;
};

namespace Instances { extern Instance<ConfigurationManager> configurationManager; }
extern ConfigurationManager& configurationManager;
