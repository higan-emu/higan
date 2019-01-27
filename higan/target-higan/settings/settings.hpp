struct UserInterfaceSettings : Setting<> {
  UserInterfaceSettings();
  auto load() -> void;
  auto save() -> void;

  struct Video : Setting<> { using Setting::Setting;
    Setting<string> driver{this, "driver"};
    Setting<boolean> blocking{this, "blocking", false};
  } video{this, "video"};

  struct Audio : Setting<> { using Setting::Setting;
    Setting<string> driver{this, "driver"};
    Setting<boolean> blocking{this, "blocking", true};
    Setting<real> frequency{this, "frequency", 48000.0};
  } audio{this, "audio"};

  struct Input : Setting<> { using Setting::Setting;
    Setting<string> driver{this, "driver"};
  } input{this, "input"};
};

struct SystemsAppendWindow : Window {
  SystemsAppendWindow();
  auto show() -> void;
  auto eventChangeSelection() -> void;
  auto eventChangeName() -> void;
  auto eventAccept() -> void;

  VerticalLayout layout{this};
    TableView systemsList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Label nameLabel{&controlLayout, Size{0, 0}};
      LineEdit nameValue{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
};

struct SystemsModifyWindow : Window {
  SystemsModifyWindow();
  auto show(string systemName) -> void;
  auto eventAccept() -> void;

  string systemName;

  VerticalLayout layout{this};
    TextEdit propertiesEdit{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button acceptButton{&controlLayout, Size{80, 0}};
};

struct SystemsPanel : TabFrameItem {
  SystemsPanel(TabFrame* parent);
  auto refresh() -> void;

  VerticalLayout layout{this};
    TableView systemsList{&layout, Size{~0, ~0}};
    HorizontalLayout controlLayout{&layout, Size{~0, 0}};
      Widget controlSpacer{&controlLayout, Size{~0, 0}};
      Button appendButton{&controlLayout, Size{80, 0}};
      Button modifyButton{&controlLayout, Size{80, 0}};
      Button removeButton{&controlLayout, Size{80, 0}};

  SystemsAppendWindow appendWindow;
  SystemsModifyWindow modifyWindow;
};

struct SettingsWindow : Window {
  SettingsWindow();
  auto show() -> void;

  VerticalLayout layout{this};
    TabFrame panels{&layout, Size{~0, ~0}};
      SystemsPanel systems{&panels};
};

extern UserInterfaceSettings settings;
namespace Instances { extern Instance<SettingsWindow> settingsWindow; }
extern SettingsWindow& settingsWindow;
