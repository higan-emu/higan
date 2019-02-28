#include "icarus.hpp"

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  directory::create({Path::userData(), "icarus/"});
  return {Path::userData(), "icarus/", name};
}

#include "settings/settings.cpp"

#include "heuristics/heuristics.cpp"
#include "heuristics/bs-memory.cpp"
#include "heuristics/colecovision.cpp"
#include "heuristics/famicom.cpp"
#include "heuristics/game-boy.cpp"
#include "heuristics/game-boy-advance.cpp"
#include "heuristics/game-gear.cpp"
#include "heuristics/master-system.cpp"
#include "heuristics/mega-drive.cpp"
#include "heuristics/msx.cpp"
#include "heuristics/neo-geo-pocket.cpp"
#include "heuristics/neo-geo-pocket-color.cpp"
#include "heuristics/pc-engine.cpp"
#include "heuristics/sc-3000.cpp"
#include "heuristics/sg-1000.cpp"
#include "heuristics/sufami-turbo.cpp"
#include "heuristics/super-famicom.cpp"
#include "heuristics/supergrafx.cpp"
#include "heuristics/wonderswan.cpp"

#include "core/core.cpp"
#include "core/bs-memory.cpp"
#include "core/colecovision.cpp"
#include "core/famicom.cpp"
#include "core/game-boy.cpp"
#include "core/game-boy-advance.cpp"
#include "core/game-boy-color.cpp"
#include "core/game-gear.cpp"
#include "core/master-system.cpp"
#include "core/mega-drive.cpp"
#include "core/msx.cpp"
#include "core/pc-engine.cpp"
#include "core/neo-geo-pocket.cpp"
#include "core/neo-geo-pocket-color.cpp"
#include "core/pocket-challenge-v2.cpp"
#include "core/sc-3000.cpp"
#include "core/sg-1000.cpp"
#include "core/sufami-turbo.cpp"
#include "core/super-famicom.cpp"
#include "core/supergrafx.cpp"
#include "core/wonderswan.cpp"
#include "core/wonderswan-color.cpp"
Icarus context;

#include "program/program.cpp"

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("icarus");

  if(auto document = file::read({Path::userSettings(), "icarus/settings.bml"})) {
    settings.unserialize(document);
  }

  if(arguments.take("--name")) {
    return print("icarus");
  }

  if(string system; arguments.take("--system", system)) {
    if(string metadata; arguments.take("--metadata", metadata)) {
      return print(context.manifest(system, metadata));
    }

    if(string import; arguments.take("--import", import)) {
      if(string output = context.import(system, import)) {
        return print(output, "\n");
      }
      return;
    }

    if(arguments.take("--import")) {
      if(auto file = BrowserDialog()
      .setTitle({"Import ", system, " Game"})
      .setPath(settings.recent)
      .setAlignment(Alignment::Center)
      .openFile()
      ) {
        if(string output = context.import(system, file)) {
          settings.recent = Location::path(file);
          directory::create({Path::userSettings(), "icarus/"});
          file::write({Path::userSettings(), "icarus/settings.bml"}, settings.serialize());
          return print(output, "\n");
        } else {
          MessageDialog()
          .setTitle("Error")
          .setAlignment(Alignment::Center)
          .setText({"Failed to import: ", Location::file(file), "\n\n", context.error(), "."})
          .error();
        }
      }
      return;
    }
  }

  Instances::programWindow.construct();

  #if defined(PLATFORM_MACOS)
  Application::Cocoa::onAbout([&] { programWindow.aboutAction.doActivate(); });
  Application::Cocoa::onPreferences([&] {});
  Application::Cocoa::onQuit([&] { Application::quit(); });
  #endif

  programWindow.setVisible();
  Application::run();

  directory::create({Path::userSettings(), "icarus/"});
  file::write({Path::userSettings(), "icarus/settings.bml"}, settings.serialize());
}
