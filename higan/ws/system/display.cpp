Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "LCD";
  node->width  = 224;
  node->height = 144;
  node->scaleX = 3.0;
  node->scaleY = 3.0;
  node->colors = 1 << 12;
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
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(colorEmulation->value()) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
}
