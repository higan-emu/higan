#include "../higan.hpp"

UserInterfaceSettings settings;
namespace Instances { Instance<SettingsWindow> settingsWindow; }
SettingsWindow& settingsWindow = Instances::settingsWindow();
#include "systems.cpp"

UserInterfaceSettings::UserInterfaceSettings() : Setting{"settings"} {
}

auto UserInterfaceSettings::load() -> void {
  unserialize(file::read(locate("configuration.bml")));
}

auto UserInterfaceSettings::save() -> void {
  file::write(locate("configuration.bml"), serialize());
}

SettingsWindow::SettingsWindow() {
  layout.setPadding(5);

  setTitle("Settings");
  setSize({600, 400});
  setAlignment({0.0, 1.0});
}

auto SettingsWindow::show() -> void {
  setVisible();
  setFocused();
  Application::processEvents();
  layout.setGeometry(layout.geometry());
}
