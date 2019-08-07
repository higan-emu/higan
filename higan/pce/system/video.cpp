auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);
  node->type    = "CRT";
  node->width   = 1120;
  node->height  =  240;
  node->scaleX  = 0.25;
  node->scaleY  = 1.00;
  node->aspectX = 8.0;
  node->aspectY = 7.0;
  node->colors  = 1 << 9;
  node->color   = [&](auto index) { return color(index); };
}

auto System::Video::color(uint32 color) -> uint64 {
  uint3 B = color.bit(0,2);
  uint3 R = color.bit(3,5);
  uint3 G = color.bit(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}
