#include "../higan.hpp"

namespace Instances { Instance<ViewportWindow> viewport; };
ViewportWindow& viewport = Instances::viewport();

auto ViewportWindow::create() -> void {
  systemMenu.setText(emulator->information().name);
  connectionsAction.setIcon(Icon::Place::Server).setText("Connections ...").onActivate([&] {
    connectionManager.show(*this);
  });
  powerToggle.setText("Power").onToggle([&] {
    if(powerToggle.checked()) program.power();
  });

  settingsMenu.setText("Settings");

  toolsMenu.setText("Tools");

  helpMenu.setText("Help");

  onClose([&] {
    Application::kill();
  });

  setTitle({"higan v", higan::Version});
  setSize({640, 480});
  setCentered();
}

auto ViewportWindow::context() -> uintptr {
  return viewport.handle();
}

auto ViewportWindow::show() -> void {
  setVisible();
  setFocused();
}
