#include "../bsnes.hpp"
#include "cheat-editor.cpp"
#include "state-manager.cpp"
#include "manifest-viewer.cpp"

Instance<CheatDatabase> cheatDatabaseInstance;
Instance<CheatWindow> cheatWindowInstance;
Instance<CheatEditor> cheatEditorInstance;
Instance<StateWindow> stateWindowInstance;
Instance<StateManager> stateManagerInstance;
Instance<ManifestViewer> manifestViewerInstance;
Instance<ToolsWindow> toolsWindowInstance;

CheatDatabase& cheatDatabase = cheatDatabaseInstance();
CheatWindow& cheatWindow = cheatWindowInstance();
CheatEditor& cheatEditor = cheatEditorInstance();
StateWindow& stateWindow = stateWindowInstance();
StateManager& stateManager = stateManagerInstance();
ManifestViewer& manifestViewer = manifestViewerInstance();
ToolsWindow& toolsWindow = toolsWindowInstance();

ToolsWindow::ToolsWindow() {
  layout.setPadding(5);
  panel.append(cheatEditor);
  panel.append(stateManager);
  panel.append(manifestViewer);
  panel.onChange([&] {
    uint offset = panel.selected().offset();
    if(offset != 0) cheatDatabase.setVisible(false), cheatWindow.setVisible(false);
    if(offset != 1) stateWindow.setVisible(false);
  });

  setTitle("Tools");
  setSize({720, 480});
  setAlignment({1.0, 1.0});
  setDismissable();

  onClose([&] {
    setVisible(false);
  });
}

auto ToolsWindow::setVisible(bool visible) -> ToolsWindow& {
  Window::setVisible(visible);
  if(!visible) {
    cheatDatabase.setVisible(false);
    cheatWindow.setVisible(false);
    stateWindow.setVisible(false);
  } else {
    Application::processEvents();
  }
  return *this;
}

auto ToolsWindow::show(uint index) -> void {
  panel.item(index).setSelected();
  setVisible();
  setFocused();
}
