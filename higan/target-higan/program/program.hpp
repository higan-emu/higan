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

struct ProgramWindow : Window {
  ProgramWindow();
  auto show(Panel&) -> void;
  auto hide(Panel&) -> void;

  MenuBar menuBar{this};
  ActionMenu actionMenu{&menuBar};
  SystemMenu systemMenu{&menuBar};
  SettingsMenu settingsMenu{&menuBar};
  ToolsMenu toolsMenu{&menuBar};
  HelpMenu helpMenu{&menuBar};

  HorizontalLayout panels{this};
    SystemManager systemManager{&panels};
    NodeManager nodeManager{&panels};
    VerticalLayout resizeLayout{&panels, Size{9, ~0}};
      HorizontalResizeGrip resizeGrip{&resizeLayout, Size{~0, ~0}};
      Button resizeSpacer{&resizeLayout, Size{0, 0}};
    Home home{&panels};
    SystemCreation systemCreation{&panels};
    SystemOverview systemOverview{&panels};
    PortConnector portConnector{&panels};
    InputMapper inputMapper{&panels};

private:
  maybe<Panel&> primaryPanel;
  maybe<Panel&> secondaryPanel;
  float resizeWidth = 0;
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
