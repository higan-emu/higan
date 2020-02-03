struct Settings : Markup::Node {
  using string = nall::string;

  auto load() -> void;
  auto save() -> void;
  auto process(bool load) -> void;

  struct Video {
    string driver;
    string monitor;
    string format;
    bool exclusive = false;
    bool blocking = false;
    bool flush = false;
    string shader = "Blur";
    uint multiplier = 2;
    string output = "Scale";
    bool aspectCorrection = true;
  } video;

  struct Audio {
    string driver;
    string device;
    uint frequency = 0;
    uint latency = 0;
    bool exclusive = false;
    bool blocking = true;
    bool dynamic = false;
    bool mute = false;
  } audio;

  struct Input {
    string driver;
  } input;

  struct General {
    bool showStatusBar = true;
  } general;
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
  auto videoRefresh() -> void;
  auto videoDriverUpdate() -> void;
  auto audioRefresh() -> void;
  auto audioDriverUpdate() -> void;
  auto inputRefresh() -> void;
  auto inputDriverUpdate() -> void;

  Label videoLabel{this, Size{~0, 0}, 2};
  HorizontalLayout videoDriverLayout{this, Size{~0, 0}};
    Label videoDriverLabel{&videoDriverLayout, Size{0, 0}};
    ComboButton videoDriverList{&videoDriverLayout, Size{0, 0}};
    Button videoDriverAssign{&videoDriverLayout, Size{0, 0}};
    Label videoDriverActive{&videoDriverLayout, Size{0, 0}};
  HorizontalLayout videoPropertyLayout{this, Size{~0, 0}};
    Label videoMonitorLabel{&videoPropertyLayout, Size{0, 0}};
    ComboButton videoMonitorList{&videoPropertyLayout, Size{0, 0}};
    Label videoFormatLabel{&videoPropertyLayout, Size{0, 0}};
    ComboButton videoFormatList{&videoPropertyLayout, Size{0, 0}};
  HorizontalLayout videoToggleLayout{this, Size{~0, 0}};
    CheckLabel videoExclusiveToggle{&videoToggleLayout, Size{0, 0}};
    CheckLabel videoBlockingToggle{&videoToggleLayout, Size{0, 0}};
    CheckLabel videoFlushToggle{&videoToggleLayout, Size{0, 0}};
  //
  Label audioLabel{this, Size{~0, 0}, 2};
  HorizontalLayout audioDriverLayout{this, Size{~0, 0}};
    Label audioDriverLabel{&audioDriverLayout, Size{0, 0}};
    ComboButton audioDriverList{&audioDriverLayout, Size{0, 0}};
    Button audioDriverAssign{&audioDriverLayout, Size{0, 0}};
    Label audioDriverActive{&audioDriverLayout, Size{0, 0}};
  HorizontalLayout audioPropertyLayout{this, Size{~0, 0}};
    Label audioDeviceLabel{&audioPropertyLayout, Size{0, 0}};
    ComboButton audioDeviceList{&audioPropertyLayout, Size{0, 0}};
    Label audioFrequencyLabel{&audioPropertyLayout, Size{0, 0}};
    ComboButton audioFrequencyList{&audioPropertyLayout, Size{0, 0}};
    Label audioLatencyLabel{&audioPropertyLayout, Size{0, 0}};
    ComboButton audioLatencyList{&audioPropertyLayout, Size{0, 0}};
  HorizontalLayout audioToggleLayout{this, Size{~0, 0}};
    CheckLabel audioExclusiveToggle{&audioToggleLayout, Size{0, 0}};
    CheckLabel audioBlockingToggle{&audioToggleLayout, Size{0, 0}};
    CheckLabel audioDynamicToggle{&audioToggleLayout, Size{0, 0}};
  //
  Label inputLabel{this, Size{~0, 0}, 2};
  HorizontalLayout inputDriverLayout{this, Size{~0, 0}};
    Label inputDriverLabel{&inputDriverLayout, Size{0, 0}};
    ComboButton inputDriverList{&inputDriverLayout, Size{0, 0}};
    Button inputDriverAssign{&inputDriverLayout, Size{0, 0}};
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
