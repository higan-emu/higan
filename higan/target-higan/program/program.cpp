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
#include "../settings/hotkeys.cpp"

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
HotkeySettings& hotkeySettings = programWindow.hotkeySettings;

ProgramWindow::ProgramWindow() {
  viewport.setFocusable();

  statusHeight = Font().size(" ").height() + 8_sy;
  layout.cell(statusLayout).setSize({~0, statusHeight});

  statusLayout.setCollapsible().setVisible(settings.interface.showStatusBar);
  statusMessage.setFont(Font().setBold());
  statusCaption.setFont(Font().setBold()).setAlignment(1.0);

  Color foreground{255, 255, 255};
  Color background{ 48,  48,  48};
  statusBefore.setForegroundColor(foreground).setBackgroundColor(background);
  statusMessage.setForegroundColor(foreground).setBackgroundColor(background);
  statusCaption.setForegroundColor(foreground).setBackgroundColor(background);
  statusAfter.setForegroundColor(foreground).setBackgroundColor(background);

  panelsHeight = 7 + 250_sy;
  verticalResizeGrip.setCollapsible().setVisible(settings.interface.showSystemPanels);
  panels.setCollapsible().setVisible(settings.interface.showSystemPanels);
  panels.setPadding({5_sx, 0_sy, 5_sx, 5_sy});

  for(auto& cell : panels.cells()) cell.setSpacing(0);

  verticalResizeGrip.onActivate([&] {
    verticalResizeHeight = layout.cell(*panels).size().height();
  }).onResize([&](auto offset) {
    float min = 128_sy, max = geometry().height() - 128_sy;
    float height = verticalResizeHeight - offset;
    height = height < min ? min : height > max ? max : height;
    if(layout.cell(*panels).size().height() != height) {
      layout.cell(*panels).setSize({~0, height});
      layout.resize();
    }
  });

  horizontalResizeGrip.onActivate([&] {
    horizontalResizeWidth = panels.cell(*primaryPanel).size().width();
  }).onResize([&](auto offset) {
    float min = 128_sx, max = panels.geometry().width() - 128_sx;
    float width = horizontalResizeWidth + offset;
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
  setSize({640_sx, 480_sy
  + (settings.interface.showStatusBar ? statusHeight : 0)
  + (settings.interface.showSystemPanels ? panelsHeight : 0)
  });
  setAlignment(Alignment::Center);
  setVisible();

  //start the ruby input driver at program startup rather than after emulator instance creation.
  //the input driver is much less likely to crash, and is needed for hotkey support to work immediately.
  emulator.inputUpdate();
  inputSettings.eventActivate();
}

auto ProgramWindow::adaptiveResize() -> void {
  if(!settings.video.adaptiveSizing) return;

  uint width  = 640;
  uint height = 480;

  if(emulator.root) {
    if(auto video = emulator.root->find<higan::Node::Video>(0)) {
      width  = video->width  * video->scaleX;
      height = video->height * video->scaleY;
      if(settings.video.aspectCorrection) width *= video->aspectX / video->aspectY;
    //if(video->rotation == 90 || video->rotation == 270) swap(width, height);
    }
  }

  uint multiplierX = ceil(640.0 / width);
  uint multiplierY = ceil(480.0 / height);
  uint multiplier  = min(multiplierX, multiplierY);

  width  *= multiplier;
  height *= multiplier;

  if(statusLayout.visible()) height += statusHeight;
  if(panels.visible()) height += panelsHeight;

  setSize({width, height});
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

auto ProgramWindow::showStatus() -> void {
  if(statusLayout.visible()) return;
  statusLayout.setVisible(true);
  if(!maximized()) setSize({geometry().width(), geometry().height() + statusHeight});
  layout.resize();
  settingsMenu.showStatusBar.setChecked(true);
}

auto ProgramWindow::hideStatus() -> void {
  if(!statusLayout.visible()) return;
  statusLayout.setVisible(false);
  if(!maximized()) setSize({geometry().width(), geometry().height() - statusHeight});
  layout.resize();
  settingsMenu.showStatusBar.setChecked(false);
}

auto ProgramWindow::showPanels() -> void {
  if(panels.visible()) return;
  verticalResizeGrip.setVisible(true);
  panels.setVisible(true);
  if(!maximized()) setSize({geometry().width(), geometry().height() + panelsHeight});
  layout.resize();
  settingsMenu.showSystemPanels.setChecked(true);
}

auto ProgramWindow::hidePanels() -> void {
  if(!panels.visible()) return;
  panelsHeight = 7 + panels.geometry().height();
  verticalResizeGrip.setVisible(false);
  panels.setVisible(false);
  if(!maximized()) setSize({geometry().width(), geometry().height() - panelsHeight});
  layout.resize();
  settingsMenu.showSystemPanels.setChecked(false);
}
