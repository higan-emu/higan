struct Panel : VerticalLayout {
  using VerticalLayout::VerticalLayout;
  virtual auto show() -> void = 0;
  virtual auto hide() -> void = 0;
};

using View = HorizontalLayout;

#include "menus.hpp"
#include "system-manager.hpp"
#include "node-manager.hpp"
#include "home.hpp"
#include "system-creation.hpp"
#include "system-overview.hpp"
#include "port-connector.hpp"
#include "input-mapper.hpp"
#include "setting-editor.hpp"
#include "../settings/video.hpp"
#include "../settings/audio.hpp"
#include "../settings/input.hpp"
#include "../settings/hotkeys.hpp"

struct ProgramWindow : Window {
  ProgramWindow();
  auto adaptiveResize() -> void;
  auto show(Panel&) -> void;
  auto hide(Panel&) -> void;
  auto showStatus() -> void;
  auto hideStatus() -> void;
  auto showPanels() -> void;
  auto hidePanels() -> void;

  MenuBar menuBar{this};
  ActionMenu actionMenu{&menuBar};
  SystemMenu systemMenu{&menuBar};
  SettingsMenu settingsMenu{&menuBar};
  ToolsMenu toolsMenu{&menuBar};
  HelpMenu helpMenu{&menuBar};

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}, 0};
    HorizontalLayout statusLayout{&layout, Size{~0, 0}, 0};
      Label statusBefore{&statusLayout, Size{8_sx, ~0}, 0};
      Label statusMessage{&statusLayout, Size{~0, ~0}, 0};
      Label statusCaption{&statusLayout, Size{100_sx, ~0}, 0};
      Label statusAfter{&statusLayout, Size{8_sx, ~0}, 0};
    VerticalResizeGrip verticalResizeGrip{&layout, Size{~0, 7}, 0};
    HorizontalLayout panels{&layout, Size{~0, 250}, 0};
      SystemManager systemManager{&panels};
      NodeManager nodeManager{&panels};
      HorizontalResizeGrip horizontalResizeGrip{&panels, Size{7, ~0}};
      Home home{&panels};
      SystemCreation systemCreation{&panels};
      SystemOverview systemOverview{&panels};
      PortConnector portConnector{&panels};
      InputMapper inputMapper{&panels};
      SettingEditor settingEditor{&panels};
      VideoSettings videoSettings{&panels};
      AudioSettings audioSettings{&panels};
      InputSettings inputSettings{&panels};
      HotkeySettings hotkeySettings{&panels};

private:
  maybe<Panel&> primaryPanel;
  maybe<Panel&> secondaryPanel;
  float verticalResizeHeight = 0;
  float horizontalResizeWidth = 0;
  float panelsHeight = 0;
  float statusHeight = 0;
};

namespace Instances { extern Instance<ProgramWindow> programWindow; }
extern ProgramWindow& programWindow;
extern ActionMenu& actionMenu;
extern SystemMenu& systemMenu;
extern SettingsMenu& settingsMenu;
extern ToolsMenu& toolsMenu;
extern HelpMenu& helpMenu;
extern SystemManager& systemManager;
extern NodeManager& nodeManager;
extern Home& home;
extern SystemCreation& systemCreation;
extern SystemOverview& systemOverview;
extern PortConnector& portConnector;
extern InputMapper& inputMapper;
extern SettingEditor& settingEditor;
extern VideoSettings& videoSettings;
extern AudioSettings& audioSettings;
extern InputSettings& inputSettings;
extern HotkeySettings& hotkeySettings;
