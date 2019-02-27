#include "../higan.hpp"
#include "menus.cpp"
#include "system-manager.cpp"
#include "node-manager.cpp"
#include "home.cpp"
#include "system-creation.cpp"
#include "system-overview.cpp"
#include "port-connector.cpp"
#include "input-mapper.cpp"
#include "setting-editor.cpp"
#include "../settings/video.cpp"
#include "../settings/audio.cpp"
#include "../settings/input.cpp"

namespace Instances { Instance<ProgramWindow> programWindow; }
ProgramWindow& programWindow = Instances::programWindow();
ActionMenu& actionMenu = programWindow.actionMenu;
SystemMenu& systemMenu = programWindow.systemMenu;
SettingsMenu& settingsMenu = programWindow.settingsMenu;
ToolsMenu& toolsMenu = programWindow.toolsMenu;
HelpMenu& helpMenu = programWindow.helpMenu;
SystemManager& systemManager = programWindow.systemManager;
NodeManager& nodeManager = programWindow.nodeManager;
Home& home = programWindow.home;
SystemCreation& systemCreation = programWindow.systemCreation;
SystemOverview& systemOverview = programWindow.systemOverview;
PortConnector& portConnector = programWindow.portConnector;
InputMapper& inputMapper = programWindow.inputMapper;
SettingEditor& settingEditor = programWindow.settingEditor;
VideoSettings& videoSettings = programWindow.videoSettings;
AudioSettings& audioSettings = programWindow.audioSettings;
InputSettings& inputSettings = programWindow.inputSettings;

ProgramWindow::ProgramWindow() {
  panels.setPadding(5_sx, 5_sy);
  for(auto& cell : panels.cells()) cell.setSpacing(0);
  resizeGrip.onActivate([&] { resizeWidth = panels.cell(*primaryPanel).size().width(); });
  resizeGrip.onResize([&](auto offset) {
    float min = 128_sx, max = panels.geometry().width() - 128_sx;
    float width = resizeWidth + offset;
    width = width < min ? min : width > max ? max : width;
    if(panels.cell(*primaryPanel).size().width() != width) {
      panels.cell(*primaryPanel).setSize({width, ~0});
      panels.resize();
    }
  });

  onClose([&] { emulator.quit(); });

  show(home);
  show(systemManager);

  setTitle({"higan v", higan::Version});
  setSize({720_sx, 360_sy});
  setAlignment(Alignment::Center);
  setVisible();
}

auto ProgramWindow::show(Panel& panel) -> void {
  if(&panel == &systemManager || &panel == &nodeManager) {
    if(primaryPanel && *primaryPanel == panel) return;
    if(primaryPanel) panels.cell(*panel).setSize(panels.cell(*primaryPanel).size());
    if(primaryPanel) primaryPanel->hide();
    primaryPanel = panel;
    primaryPanel->show();
    if(&panel == &systemManager) {
      systemMenu.setVisible(false);
      toolsMenu.setVisible(false);
      //todo: if test shouldn't be necessary ... works around a hiro/GTK bug ...
      if(!actionMenu.visible()) actionMenu.setVisible(true);
    }
    if(&panel == &nodeManager) {
      actionMenu.setVisible(false);
      systemMenu.setVisible(true);
      toolsMenu.setVisible(true);
    }
    return (void)panels.resize();
  }

  if(secondaryPanel && *secondaryPanel == panel) return;
  if(secondaryPanel) secondaryPanel->hide();
  secondaryPanel = panel;
  secondaryPanel->show();
  panels.resize();
}

auto ProgramWindow::hide(Panel& panel) -> void {
  show(home);
}
