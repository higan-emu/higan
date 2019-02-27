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
  setAlignment(Alignment::Center);
  setVisible();
  setFocused();

  if(!video) {
    video.create(settings.video.driver);
    video.setContext(viewport.handle());
    video.setBlocking(false);
  }
}
