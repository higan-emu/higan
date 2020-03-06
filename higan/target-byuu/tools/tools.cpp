#include "../byuu.hpp"
#include "tracer.cpp"

namespace Instances { Instance<ToolsWindow> toolsWindow; }
ToolsWindow& toolsWindow = Instances::toolsWindow();
TraceLogger& traceLogger = toolsWindow.traceLogger;

ToolsWindow::ToolsWindow() {
  layout.setPadding(5_sx, 5_sy);

  panelList.append(ListViewItem().setText("Tracer").setIcon(Icon::Emblem::Text));
  panelList.onChange([&] { eventChange(); });

  panelContainer.append(traceLogger, Size{~0, ~0});
  panelContainer.append(homePanel, Size{~0, ~0});

  traceLogger.construct();
  homePanel.construct();

  setDismissable();
  setTitle("Tools");
  setSize({720, 400});
  setAlignment({1.0, 1.0});
}

auto ToolsWindow::show(const string& panel) -> void {
  for(auto& item : panelList.items()) {
    if(item.text() == panel) {
      item.setSelected();
      eventChange();
      break;
    }
  }
  setVisible();
  setFocused();
}

auto ToolsWindow::eventChange() -> void {
  traceLogger.setVisible(false);
  homePanel.setVisible(false);

  bool found = false;
  if(auto item = panelList.selected()) {
    if(item.text() == "Tracer") found = true, traceLogger.setVisible();
  }
  if(!found) homePanel.setVisible();

  panelContainer.resize();
}
