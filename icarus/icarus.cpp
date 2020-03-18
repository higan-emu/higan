#include "icarus.hpp"

namespace icarus {

vector<shared_pointer<Media>> media;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  directory::create({Path::userData(), "icarus/"});
  return {Path::userData(), "icarus/", name};
}

auto operator+=(string& lhs, const string& rhs) -> string& {
  lhs.append(rhs);
  return lhs;
}

#include "settings/settings.cpp"
#include "media/media.cpp"
#include "cartridge/cartridge.cpp"
#include "compact-disc/compact-disc.cpp"
#include "floppy-disk/floppy-disk.cpp"
#include "program/program.cpp"

auto construct() -> void {
  media.append(new BSMemory);
  media.append(new ColecoVision);
  media.append(new Famicom);
  media.append(new FamicomDisk);
  media.append(new GameBoy);
  media.append(new GameBoyAdvance);
  media.append(new GameBoyColor);
  media.append(new GameGear);
  media.append(new MasterSystem);
  media.append(new MegaCD);
  media.append(new MegaDrive);
  media.append(new MSX);
  media.append(new MSX2);
  media.append(new NeoGeoPocket);
  media.append(new NeoGeoPocketColor);
  media.append(new PCEngine);
  media.append(new PCEngineCD);
  media.append(new PocketChallengeV2);
  media.append(new SC3000);
  media.append(new SG1000);
  media.append(new SufamiTurbo);
  media.append(new SuperFamicom);
  media.append(new SuperGrafx);
  media.append(new WonderSwan);
  media.append(new WonderSwanColor);
  for(auto& medium : media) medium->construct();
}

auto main(Arguments arguments) -> void {
  Application::setName("icarus");

  construct();

  if(auto document = file::read({Path::userSettings(), "icarus/settings.bml"})) {
    settings.unserialize(document);
  }

  if(arguments.take("--name")) {
    return print("icarus");
  }

  if(string system; arguments.take("--system", system)) {
    for(auto& medium : media) {
      if(medium->name() != system) continue;

      if(string manifest; arguments.take("--manifest", manifest)) {
        return print(medium->manifest(manifest));
      }

      if(string import; arguments.take("--import", import)) {
        return (void)medium->import(import);
      }

      if(arguments.take("--import")) {
        if(auto import = BrowserDialog()
        .setTitle({"Import ", system, " Game"})
        .setPath(settings.recent)
        .setAlignment(Alignment::Center)
        .openFile()
        ) {
          if(auto error = medium->import(import)) {
            MessageDialog()
            .setTitle("Error")
            .setAlignment(Alignment::Center)
            .setText({"Failed to import: ", Location::file(import), "\n\nError: ", error, "."})
            .error();
          }
        }
        return;
      }
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

}

#if !defined(ICARUS_LIBRARY)

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  icarus::main(arguments);
}

#endif
