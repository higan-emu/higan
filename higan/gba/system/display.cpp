Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "CRT";
  node->width  = 240;
  node->height = 160;
  node->scaleX = 3.0;
  node->scaleY = 3.0;
  node->colors = 1 << 15;
  node->color  = [&](auto index) { return color(index); };
  parent->append(node);

  colorEmulation = Node::Boolean::create("Color Emulation", true, [&](auto value) {
    if(screen) screen->setPalette();
  });
  colorEmulation->dynamic = true;
  node->append(colorEmulation);

  interframeBlending = Node::Boolean::create("Interframe Blending", true, [&](auto value) {
    if(screen) screen->setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;
  node->append(interframeBlending);

  orientation = Node::String::create("Orientation", "Horizontal", [&](auto value) {
    if(screen) screen->setRotateLeft(value == "Vertical");
  });
  orientation->dynamic = true;
  orientation->allowedValues = {"Horizontal", "Vertical"};
  node->append(orientation);

  Node::load(node, from);
}

auto Display::color(uint32 color) -> uint64 {
  uint R = color.bits( 0, 4);
  uint G = color.bits( 5, 9);
  uint B = color.bits(10,14);

  uint64 r = image::normalize(R, 5, 16);
  uint64 g = image::normalize(G, 5, 16);
  uint64 b = image::normalize(B, 5, 16);

  if(colorEmulation->value()) {
    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
  }

  return r << 32 | g << 16 | b << 0;
}
