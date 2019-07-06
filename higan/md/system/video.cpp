auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);

  node->type   = "CRT";
  node->width  = 1280;
  node->height =  480;
  node->scaleX = 0.25;
  node->scaleY = 0.50;
  node->colors = 3 * (1 << 9);
  node->color  = [&](auto index) { return color(index); };

  display = Node::append<Node::String>(parent, from, "Display", "PAL", [&](auto region) {
    if(region == "NTSC") node->height = 448;
    if(region == "PAL" ) node->height = 480;
  });
  display->setAllowedValues({"NTSC", "PAL"});
  display->dynamic = true;
}

auto System::Video::color(uint32 color) -> uint64 {
  uint R = color.bits(0, 2);
  uint G = color.bits(3, 5);
  uint B = color.bits(6, 8);
  uint M = color.bits(9,10);

  uint lookup[3][8] = {
    {  0,  29,  52,  70,  87, 101, 116, 130},  //shadow
    {  0,  52,  87, 116, 144, 172, 206, 255},  //normal
    {130, 144, 158, 172, 187, 206, 228, 255},  //highlight
  };

  uint64 r = image::normalize(lookup[M][R], 8, 16);
  uint64 g = image::normalize(lookup[M][G], 8, 16);
  uint64 b = image::normalize(lookup[M][B], 8, 16);

  return r << 32 | g << 16 | b << 0;
}
