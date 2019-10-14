struct PanelList : VerticalLayout {
  using VerticalLayout::VerticalLayout;
  virtual auto show() -> void = 0;
  virtual auto hide() -> void = 0;
  virtual auto refresh() -> void {}
};

struct PanelItem : VerticalLayout {
  using VerticalLayout::VerticalLayout;
  virtual auto show() -> void = 0;
  virtual auto hide() -> void = 0;
  virtual auto refresh() -> void {}
};

using View = HorizontalLayout;

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
#include "../panel-lists/settings-manager.hpp"
#include "../panel-lists/system-manager.hpp"
#include "../panel-lists/node-manager.hpp"
#include "../panel-lists/port-manager.hpp"

struct Program : Window {
  Program();
  auto resize() -> void;
  auto setOverviewMode() -> void;
  auto setEmulatorMode() -> void;
  auto setPanelList(PanelList&) -> void;
  auto setPanelItem(PanelItem&) -> void;
  auto refreshPanelList() -> void;
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
    HorizontalLayout panelLayout{&layout, Size{~0, 250_sy}, 0};
      VerticalLayout panelBlock{&panelLayout, Size{200_sx, ~0}};
        ComboButton panelGroup{&panelBlock, Size{~0, 0}};
        HorizontalLayout panelLists{&panelBlock, Size{~0, ~0}};
          SettingsManager settingsManager{&panelLists};
          SystemManager systemManager{&panelLists};
          NodeManager nodeManager{&panelLists};
          PortManager portManager{&panelLists};
      HorizontalResizeGrip horizontalResizeGrip{&panelLayout, Size{7, ~0}};
      Home home{&panelLayout};
      SystemCreation systemCreation{&panelLayout};
      SystemOverview systemOverview{&panelLayout};
      PortConnector portConnector{&panelLayout};
      InputMapper inputMapper{&panelLayout};
      SettingEditor settingEditor{&panelLayout};
      VideoSettings videoSettings{&panelLayout};
      AudioSettings audioSettings{&panelLayout};
      InputSettings inputSettings{&panelLayout};
      HotkeySettings hotkeySettings{&panelLayout};

private:
  maybe<PanelList&> activePanelList;
  maybe<PanelItem&> activePanelItem;
  float verticalResizeHeight = 0;
  float horizontalResizeWidth = 0;
  float panelHeight = 0;
  float statusHeight = 0;
};

namespace Instances { extern Instance<Program> program; }
extern Program& program;
extern ActionMenu& actionMenu;
extern SystemMenu& systemMenu;
extern SettingsMenu& settingsMenu;
extern ToolsMenu& toolsMenu;
extern HelpMenu& helpMenu;
extern SettingsManager& settingsManager;
extern SystemManager& systemManager;
extern NodeManager& nodeManager;
extern PortManager& portManager;
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
