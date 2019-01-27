#include "higan.hpp"
Video video;
Audio audio;
Input input;
shared_pointer<higan::Interface> emulator;
vector<shared_pointer<higan::Interface>> emulators;

auto locate(string name) -> string {
  string location = {Path::program(), name};
  if(inode::exists(location)) return location;

  directory::create({Path::userData(), "higan/"});
  return {Path::userData(), "higan/", name};
}

auto hiro::initialize() -> void {
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("higan");
  Application::setScreenSaver(false);
  settings.load();
  settings.save();  //creates settings file if it does not exist
  program.construct(arguments);
  Application::run();
}
