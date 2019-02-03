#include "../higan.hpp"

auto ViewportWindow::create(higan::Node::Video node) -> void {
  this->node = node;

  onClose([&] {
    program.quit();
  });

  setTitle(node->name);
  setSize({node->width * node->aspect, node->height});
}

auto ViewportWindow::show(Window& parent) -> void {
  setFramePosition({
    parent.frameGeometry().x() + parent.frameGeometry().width(),
    parent.frameGeometry().y()
  });
  setVisible();
  setFocused();

  if(!video) {
    video.create("OpenGL 3.2");
    video.setContext(viewport.handle());
    video.setBlocking(false);
  }
}
