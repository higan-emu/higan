Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "CRT";
  node->width  = 1120;
  node->height =  240;
  node->aspect = 8.0 / 7.0;
  node->colors = 1 << 9;
  node->color  = [&](auto index) { return color(index); };
  parent->append(node);

  Node::load(node, from);
}

auto Display::color(uint32 color) -> uint64 {
  uint3 B = color.bits(0,2);
  uint3 R = color.bits(3,5);
  uint3 G = color.bits(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}
