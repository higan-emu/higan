auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);

  if(MSX::Model::MSX()) {
    node->type   = "CRT";
    node->width  = 256;
    node->height = 192;
    node->scaleX = 2.0;
    node->scaleY = 2.0;
    node->colors = 1 << 4;
    node->color  = [&](auto index) { return colorMSX(index); };
  }

  if(MSX::Model::MSX2()) {
    node->type   = "CRT";
    node->width  = 512;
    node->height = 424;
    node->scaleX = 1.0;
    node->scaleY = 1.0;
    node->colors = 1 << 9;
    node->color  = [&](auto index) { return colorMSX2(index); };
  }
}

auto System::Video::colorMSX(uint32 color) -> uint64 {
  switch(color.bits(0,3)) {
  case  0: return 0x0000'0000'0000ull;  //transparent
  case  1: return 0x0000'0000'0000ull;  //black
  case  2: return 0x2121'c8c8'4242ull;  //medium green
  case  3: return 0x5e5e'dcdc'7878ull;  //light green
  case  4: return 0x5454'5555'ededull;  //dark blue
  case  5: return 0x7d7d'7676'fcfcull;  //light blue
  case  6: return 0xd4d4'5252'4d4dull;  //dark red
  case  7: return 0x4242'ebeb'f5f5ull;  //cyan
  case  8: return 0xfcfc'5555'5454ull;  //medium red
  case  9: return 0xffff'7979'7878ull;  //light red
  case 10: return 0xd4d4'c1c1'5454ull;  //dark yellow
  case 11: return 0xe6e6'cece'8080ull;  //light yellow
  case 12: return 0x2121'b0b0'3b3bull;  //dark green
  case 13: return 0xc9c9'5b5b'babaull;  //magenta
  case 14: return 0xcccc'cccc'ccccull;  //gray
  case 15: return 0xffff'ffff'ffffull;  //white
  }
  unreachable;
}

auto System::Video::colorMSX2(uint32 color) -> uint64 {
  uint3 B = color.bits(0,2);
  uint3 R = color.bits(3,5);
  uint3 G = color.bits(6,8);

  uint64 r = image::normalize(R, 3, 16);
  uint64 g = image::normalize(G, 3, 16);
  uint64 b = image::normalize(B, 3, 16);

  return r << 32 | g << 16 | b << 0;
}
