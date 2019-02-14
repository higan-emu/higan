Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type = Model::GameGear() ? "LCD" : "CRT";

  if(Model::ColecoVision() || Model::SG1000() || Model::SC3000()) {
    node->width = 256;
    node->height = 192;
    node->aspect = 1.0;
    node->colors = 1 << 4;
    node->color = [&](auto index) { return color(index); };
  }

  if(Model::MasterSystem()) {
    node->width = 256;
    node->height = 240;
    node->aspect = 8.0 / 7.0;
    node->colors = 1 << 6;
    node->color = [&](auto index) { return colorMasterSystem(index); };
  }

  if(Model::GameGear()) {
    node->width = 160;
    node->height = 144;
    node->aspect = 1.0;
    node->colors = 1 << 12;
    node->color = [&](auto index) { return colorGameGear(index); };
  }

  parent->append(node);
}

auto Display::color(uint32 color) -> uint64 {
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
