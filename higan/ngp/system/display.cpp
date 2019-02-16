Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "LCD";
  node->width  = 160;
  node->height = 152;
  node->aspect = 1.0;
  node->colors = 1 << 12;
  node->color  = [&](auto index) { return color(index); };
  parent->append(node);

  interframeBlending = Node::Boolean::create("Interframe Blending", true, [&](auto value) {
    screen->setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;
  node->append(interframeBlending);

  Node::load(node, from);
}

auto Display::color(uint32 color) -> uint64 {
  uint b = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint r = color.bits(8,11);

  natural R = image::normalize(r, 4, 16);
  natural G = image::normalize(g, 4, 16);
  natural B = image::normalize(b, 4, 16);

  return R << 32 | G << 16 | B << 0;
}
