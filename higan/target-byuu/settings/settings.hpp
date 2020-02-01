struct Settings : Markup::Node {
  using string = nall::string;

  auto load() -> void;
  auto save() -> void;
  auto process(bool load) -> void;

  struct Video {
    string driver;
  } video;

  struct Audio {
    string driver;
    bool mute = false;
  } audio;

  struct Input {
    string driver;
  } input;
};

struct InputSettings : VerticalLayout {
  InputSettings();
  auto reload() -> void;

  TableView inputList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    Button assignButton{&controlLayout, Size{80, 0}};
    Button clearButton{&controlLayout, Size{80, 0}};
};

struct DriverSettings : VerticalLayout {
  DriverSettings();

  Button button{this, Size{80, 0}};
};

struct SettingsWindow : Window {
  SettingsWindow();
  auto show(const string& panel) -> void;
  auto eventChange() -> void;

  HorizontalLayout layout{this};
    ListView panelList{&layout, Size{125_sx, ~0}};
    VerticalLayout panelContainer{&layout, Size{~0, ~0}};
      InputSettings inputSettings;
      DriverSettings driverSettings;

  StatusBar statusBar{this};
};

extern Settings settings;
namespace Instances { extern Instance<SettingsWindow> settingsWindow; }
extern SettingsWindow& settingsWindow;
extern InputSettings& inputSettings;
extern DriverSettings& driverSettings;
