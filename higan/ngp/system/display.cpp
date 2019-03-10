Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "LCD";
  node->width  = 160;
  node->height = 152;
  node->scaleX = 3.0;
  node->scaleY = 3.0;
  parent->append(node);

  if(Model::NeoGeoPocket()) {
    node->colors = 1 << 3;
    node->color  = [&](auto index) { return colorNeoGeoPocket(index); };
  }

  if(Model::NeoGeoPocketColor()) {
    node->colors = 1 << 12;
    node->color  = [&](auto index) { return colorNeoGeoPocketColor(index); };
  }

  interframeBlending = Node::Boolean::create("Interframe Blending", true, [&](auto value) {
    if(screen) screen->setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;
  node->append(interframeBlending);

  Node::load(node, from);
}

auto Display::colorNeoGeoPocket(uint32 color) -> uint64 {
  uint3 l = color.bits(0,2);

  natural L = image::normalize(7 - l, 3, 16);

  return L << 32 | L << 16 | L << 0;
}

auto Display::colorNeoGeoPocketColor(uint32 color) -> uint64 {
  uint r = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint b = color.bits(8,11);

  natural R = image::normalize(r, 4, 16);
  natural G = image::normalize(g, 4, 16);
  natural B = image::normalize(b, 4, 16);

  return R << 32 | G << 16 | B << 0;
}
