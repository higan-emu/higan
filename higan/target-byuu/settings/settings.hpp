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

struct VideoSettings : VerticalLayout {
  VideoSettings();

  Label colorAdjustmentLabel{this, Size{~0, 0}, 2};
};

struct AudioSettings : VerticalLayout {
  AudioSettings();

  Label effectsLabel{this, Size{~0, 0}, 2};
};

struct InputSettings : VerticalLayout {
  InputSettings();
  auto reload() -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventClear() -> void;
  auto eventAssign() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void;

  TableView inputList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    Button assignButton{&controlLayout, Size{80, 0}};
    Button clearButton{&controlLayout, Size{80, 0}};

  maybe<InputMapping&> activeMapping;
};

struct HotkeySettings : VerticalLayout {
  HotkeySettings();
  auto reload() -> void;
  auto refresh() -> void;
  auto eventChange() -> void;
  auto eventClear() -> void;
  auto eventAssign() -> void;
  auto eventInput(shared_pointer<HID::Device>, uint groupID, uint inputID, int16_t oldValue, int16_t newValue) -> void;

  TableView inputList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Widget spacer{&controlLayout, Size{~0, 0}};
    Button assignButton{&controlLayout, Size{80, 0}};
    Button clearButton{&controlLayout, Size{80, 0}};

  maybe<InputMapping&> activeMapping;
};

struct DriverSettings : VerticalLayout {
  DriverSettings();

  Label videoLabel{this, Size{~0, 0}, 2};
  VerticalLayout videoLayout{this, Size{~0, 0}};
    HorizontalLayout videoDriverLayout{&videoLayout, Size{~0, 0}};
      Label videoDriverLabel{&videoDriverLayout, Size{0, 0}};
      ComboButton videoDriverList{&videoDriverLayout, Size{0, 0}};
      Button videoDriverUpdate{&videoDriverLayout, Size{0, 0}};
      Label videoDriverActive{&videoDriverLayout, Size{0, 0}};
  //
  Label audioLabel{this, Size{~0, 0}, 2};
  VerticalLayout audioLayout{this, Size{~0, 0}};
    HorizontalLayout audioDriverLayout{&audioLayout, Size{~0, 0}};
      Label audioDriverLabel{&audioDriverLayout, Size{0, 0}};
      ComboButton audioDriverList{&audioDriverLayout, Size{0, 0}};
      Button audioDriverUpdate{&audioDriverLayout, Size{0, 0}};
      Label audioDriverActive{&audioDriverLayout, Size{0, 0}};
  //
  Label inputLabel{this, Size{~0, 0}, 2};
  VerticalLayout inputLayout{this, Size{~0, 0}};
    HorizontalLayout inputDriverLayout{&inputLayout, Size{~0, 0}};
      Label inputDriverLabel{&inputDriverLayout, Size{0, 0}};
      ComboButton inputDriverList{&inputDriverLayout, Size{0, 0}};
      Button inputDriverUpdate{&inputDriverLayout, Size{0, 0}};
      Label inputDriverActive{&inputDriverLayout, Size{0, 0}};
};

struct SettingsWindow : Window {
  SettingsWindow();
  auto show(const string& panel) -> void;
  auto eventChange() -> void;

  HorizontalLayout layout{this};
    ListView panelList{&layout, Size{125_sx, ~0}};
    VerticalLayout panelContainer{&layout, Size{~0, ~0}};
      VideoSettings videoSettings;
      AudioSettings audioSettings;
      InputSettings inputSettings;
      HotkeySettings hotkeySettings;
      DriverSettings driverSettings;

  StatusBar statusBar{this};
};

extern Settings settings;
namespace Instances { extern Instance<SettingsWindow> settingsWindow; }
extern SettingsWindow& settingsWindow;
extern VideoSettings& videoSettings;
extern AudioSettings& audioSettings;
extern InputSettings& inputSettings;
extern HotkeySettings& hotkeySettings;
extern DriverSettings& driverSettings;
