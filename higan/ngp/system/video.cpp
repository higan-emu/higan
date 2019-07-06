auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);
  node->type   = "LCD";
  node->width  = 160;
  node->height = 152;
  node->scaleX = 1.5;
  node->scaleY = 1.5;

  if(NeoGeoPocket::Model::NeoGeoPocket()) {
    node->colors = 1 << 3;
    node->color  = [&](auto index) { return colorNeoGeoPocket(index); };
  }

  if(NeoGeoPocket::Model::NeoGeoPocketColor()) {
    node->colors = 1 << 12;
    node->color  = [&](auto index) { return colorNeoGeoPocketColor(index); };
  }

  interframeBlending = Node::append<Node::Boolean>(parent, from, "Interframe Blending", true, [&](auto value) {
    vpu.display.setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;
}

auto System::Video::colorNeoGeoPocket(uint32 color) -> uint64 {
  uint3 l = color.bits(0,2);

  natural L = image::normalize(7 - l, 3, 16);

  return L << 32 | L << 16 | L << 0;
}

auto System::Video::colorNeoGeoPocketColor(uint32 color) -> uint64 {
  uint r = color.bits(0, 3);
  uint g = color.bits(4, 7);
  uint b = color.bits(8,11);

  natural R = image::normalize(r, 4, 16);
  natural G = image::normalize(g, 4, 16);
  natural B = image::normalize(b, 4, 16);

  return R << 32 | G << 16 | B << 0;
}
