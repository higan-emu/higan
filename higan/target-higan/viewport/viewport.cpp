#include "../higan.hpp"

auto ViewportWindow::create(higan::Node::Video node) -> void {
  this->node = node;

  onClose([&] {
    emulator.power(false);
  });

  setTitle(node->name);
  setSize({
    node->width * node->scaleX * (node->aspectX / node->aspectY),
    node->height * node->scaleY
  });
}

auto ViewportWindow::show(Window parent) -> void {
  //currently, no systems are emulated that use more than one display
  //this code will snap the program window (parent) and lone display together,
  //in the center of the screen. something more complex will be necessary to
  //position two or more displays onscreen at a time.

  auto geometryW = Desktop::workspace();
  auto geometryL = parent.frameGeometry();
  auto geometryR = frameGeometry();

  int LX = (geometryW.width() - (geometryL.width() + geometryR.width())) * 0.5;
  int LY = (geometryW.height() - geometryL.height()) * 0.5;
  parent.setFramePosition({LX, LY});

  int RX = LX + geometryL.width();
  int RY = (geometryW.height() - geometryR.height()) * 0.5;
  setFramePosition({RX, RY});

  setVisible();
  setFocused();
  Application::processEvents();

  if(!video) {
    video.create(settings.video.driver);
    video.setContext(viewport.handle());
    video.setBlocking(false);

    //now that a video driver has been initialized,
    //populate video settings if it hasn't already been
    videoSettings.eventActivate();
  }
}
