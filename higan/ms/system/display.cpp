Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");

  if(Model::MasterSystem()) {
    node->type   = "CRT";
    node->width  = 256;
    node->height = 240;
    node->aspect = 8.0 / 7.0;
    node->colors = 1 << 6;
    node->color  = [&](auto index) { return colorMasterSystem(index); };
  }

  if(Model::GameGear()) {
    node->type   = "LCD";
    node->width  = 160;
    node->height = 144;
    node->aspect = 1.0;
    node->colors = 1 << 12;
    node->color  = [&](auto index) { return colorGameGear(index); };
  }

  parent->append(node);
}

auto Display::colorMasterSystem(uint32 color) -> uint64 {
  uint2 B = color >> 4;
  uint2 G = color >> 2;
  uint2 R = color >> 0;

  uint64 r = image::normalize(R, 2, 16);
  uint64 g = image::normalize(G, 2, 16);
  uint64 b = image::normalize(B, 2, 16);

  return r << 32 | g << 16 | b << 0;
}

auto Display::colorGameGear(uint32 color) -> uint64 {
  uint4 B = color >> 8;
  uint4 G = color >> 4;
  uint4 R = color >> 0;

  uint64 r = image::normalize(R, 4, 16);
  uint64 g = image::normalize(G, 4, 16);
  uint64 b = image::normalize(B, 4, 16);

  return r << 32 | g << 16 | b << 0;
}
