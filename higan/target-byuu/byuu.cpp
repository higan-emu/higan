#include "byuu.hpp"

namespace ruby {
  Video video;
  Audio audio;
  Input input;
}

#include <nall/main.hpp>
auto nall::main(Arguments arguments) -> void {
  Application::setName("byuu");
  Application::setScreenSaver(false);

  Emulator::construct();
  Instances::presentation.construct();

  if(!Path::user().find("byuu")) {
    MessageDialog()
    .setTitle("byuu")
    .setText({
      "Note: this is pre-alpha software!\n"
      "Development literally just started.\n\n"
      "Please do not distribute binaries of byuu.\n"
      "Please do not report any bugs.\n"
      "Please don't even run this at all!\n"
      "If you do so anyway, the consequences are on you."
    })
    .information();
  }

  program.create();
  Application::onMain({&Program::main, &program});
  Application::run();

  Instances::presentation.destruct();
}
