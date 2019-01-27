#include "../higan.hpp"

namespace Instances { Instance<Presentation> presentation; }
Presentation& presentation = Instances::presentation();
#include "load.cpp"

Presentation::Presentation() {
  systemsMenu.setText("Systems");
  refreshSystems();

  systemMenu.setVisible(false);

  settingsMenu.setText("Settings");
  settingsSystems.setIcon(Icon::Device::Storage).setText("Systems ...").onActivate([&] {
    settingsWindow.show();
  });

  helpMenu.setText("Help");
  documentation.setIcon(Icon::Application::Browser).setText("Documentation ...").onActivate([&] {
    invoke("https://doc.byuu.org/higan/");
  });
  about.setIcon(Icon::Prompt::Question).setText("About ...").onActivate([&] {
    AboutDialog()
    .setLogo(Resource::Logo)
    .setVersion(higan::Version)
    .setAuthor(higan::Author)
    .setLicense(higan::License)
    .setWebsite(higan::Website)
    .setParent(*this).show();
  });

  verticalLayout.setCollapsible();

  image icon{Resource::Icon};
  icon.alphaBlend(0x000000);
  programIcon.setIcon(icon);

  auto font = Font().setBold();
  auto backgroundColor = Color{ 32,  32,  32};
  auto foregroundColor = Color{255, 255, 255};

  spacerIcon.setBackgroundColor(backgroundColor);
  statusIcon.setColor(backgroundColor);
  spacerLeft.setBackgroundColor(backgroundColor);
  statusMessage.setFont(font).setBackgroundColor(backgroundColor).setForegroundColor(foregroundColor).setAlignment(0.0);
  statusMonitor.setFont(font).setBackgroundColor(backgroundColor).setForegroundColor(foregroundColor).setAlignment(1.0);
  spacerRight.setBackgroundColor(backgroundColor);

  setTitle({"higan v", higan::Version});
  setBackgroundColor({0, 0, 0});
  setSize({640, 480});
  setCentered();

  onClose([&] {
    program.quit();
  });
}

auto Presentation::refreshSystems() -> void {
  systemsMenu.reset();

  auto location = locate("Systems/");
  for(auto system : directory::contents(location, "*.sys")) {
    auto document = BML::unserialize(file::read({location, system, "properties.bml"}));
    if(auto name = document["system/name"].text()) {
      for(auto emulator : emulators) {
        if(name == emulator->information().name) {
          MenuItem menu{&systemsMenu};
          menu.setIcon(Icon::Device::Storage).setText(system.trimRight(".sys/", 1L)).onActivate([=] {
            program.activate(emulator);
          });
          break;
        }
      }
    }
  }

  if(systemsMenu.actionCount()) systemsMenu.append(MenuSeparator());
  systemsMenu.append(MenuItem().setIcon(Icon::Action::Quit).setText("Quit").onActivate([&] {
    doClose();
  }));
}

auto Presentation::activate() -> void {
  systemsMenu.setVisible(false);
  systemMenu.setText(emulator->information().name);
  systemMenu.append(MenuItem().setIcon(Icon::Action::Open).setText("Load ...").onActivate([&] {
    loadWindow.show();
  }));
  for(auto& port : emulator->ports()) {
    Menu menu{&systemMenu};
    menu.setProperty("portID", port.id);
    menu.setText(port.name);
    if(port.name.beginsWith("Controller")) {
      menu.setIcon(Icon::Device::Joypad);
    } else {
      menu.setIcon(Icon::Device::Storage);
    }

    Group devices;
    for(auto& device : emulator->devices(port.id)) {
      MenuRadioItem item{&menu};
      item.setProperty("deviceID", device.id);
      item.setText(device.name);
      item.onActivate([=] {
      });
      devices.append(item);
    }

    menu.append(MenuSeparator());
    menu.append(MenuItem().setIcon(Icon::Action::Settings).setText("Configure ...").onActivate([&] {
    }));
  }
  if(emulator->ports()) {
    systemMenu.append(MenuSeparator());
  }
  systemMenu.append(powerStatus = MenuCheckItem().setText("Power").onToggle([&] {
    program.power(powerStatus.checked());
  }));
  systemMenu.append(MenuSeparator());
  systemMenu.append(MenuItem().setIcon(Icon::Action::Quit).setText("Return").onActivate([&] {
    program.deactivate();
  }));
  systemMenu.setVisible(true);
  power(false);
}

auto Presentation::deactivate() -> void {
  power(false);
  systemMenu.setVisible(false).reset();
  systemsMenu.setVisible();
  statusIcon.setColor({32, 32, 32});
  statusMessage.setText();
  setTitle({"higan v", higan::Version});
}

auto Presentation::power(bool power) -> void {
  verticalLayout.setVisible(!power);
  layout.setGeometry(layout.geometry());
  if(power) {
    setTitle(emulator->titles().merge(" + "));
    setStatusIcon(Icon::Media::Play);
    statusMessage.setText();
    loadWindow.bootButton.setEnabled(false);
  } else {
    setTitle(emulator->information().name);
    clearViewport();
    setStatusIcon(Icon::Media::Stop);
    statusMessage.setText("Power Off");
    statusMonitor.setText();
    loadWindow.bootButton.setEnabled(true);
  }
}

auto Presentation::load() -> void {
  verticalLayout.setVisible(false);
  layout.setGeometry(layout.geometry());
  setStatusIcon(Icon::Media::Play);
  statusMessage.setText();
  setTitle(emulator->titles().merge(" + "));
}

auto Presentation::unload() -> void {
  verticalLayout.setVisible(true);
  layout.setGeometry(layout.geometry());
}

auto Presentation::clearViewport() -> void {
  if(auto [output, length] = video.acquire(16, 16); output) {
    length >>= 2;
    for(uint y : range(16)) {
      memory::fill<uint32>(output + y * length, 16);
    }
    video.release();
    video.output();
  }
}

auto Presentation::setStatusIcon(image icon) -> void {
  image output;
  output.allocate(16, 24);
  output.fill(0xff202020);
  output.impose(image::blend::sourceAlpha, 0, 4, icon, 0, 0, 16, 16);
  statusIcon.setIcon(output);
}
