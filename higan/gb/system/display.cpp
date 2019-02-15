Display display;

auto Display::load(Node::Object parent, Node::Object from) -> void {
  node = Node::Video::create("Display");
  node->type   = "LCD";
  node->width  = 160;
  node->height = 144;
  node->aspect = 1.0;
  parent->append(node);

  if(Model::GameBoy()) {
    node->colors = 1 << 2;
    node->color  = [&](auto index) { return colorGameBoy(index); };
  }

  if(Model::GameBoyColor()) {
    node->colors = 1 << 15;
    node->color  = [&](auto index) { return colorGameBoyColor(index); };
  }

  colorEmulation = Node::Boolean::create("Color Emulation", true, [&](auto value) {
    screen->setPalette();
  });
  colorEmulation->dynamic = true;
  node->append(colorEmulation);

  interframeBlending = Node::Boolean::create("Interframe Blending", true, [&](auto value) {
    screen->setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;
  node->append(interframeBlending);

  Node::load(node, from);
}

auto Display::colorGameBoy(uint32 color) -> uint64 {
  if(!colorEmulation->value()) {
    uint64 L = image::normalize(3 - color, 2, 16);
    return L << 32 | L << 16 | L << 0;
  } else {
    #define DMG_PALETTE_GREEN
  //#define DMG_PALETTE_YELLOW
  //#define DMG_PALETTE_WHITE

    const uint16 monochrome[4][3] = {
      #if defined(DMG_PALETTE_GREEN)
      {0xaeae, 0xd9d9, 0x2727},
      {0x5858, 0xa0a0, 0x2828},
      {0x2020, 0x6262, 0x2929},
      {0x1a1a, 0x4545, 0x2a2a},
      #elif defined(DMG_PALETTE_YELLOW)
      {0xffff, 0xf7f7, 0x7b7b},
      {0xb5b5, 0xaeae, 0x4a4a},
      {0x6b6b, 0x6969, 0x3131},
      {0x2121, 0x2020, 0x1010},
      #elif defined(DMG_PALETTE_WHITE)
      {0xffff, 0xffff, 0xffff},
      {0xaaaa, 0xaaaa, 0xaaaa},
      {0x5555, 0x5555, 0x5555},
      {0x0000, 0x0000, 0x0000},
      #endif
    };

    uint64 R = monochrome[color][0];
    uint64 G = monochrome[color][1];
    uint64 B = monochrome[color][2];

    return R << 32 | G << 16 | B << 0;
  }
}

auto Display::colorGameBoyColor(uint32 color) -> uint64 {
  uint r = color.bits( 0, 4);
  uint g = color.bits( 5, 9);
  uint b = color.bits(10,14);

  uint64_t R = image::normalize(r, 5, 16);
  uint64_t G = image::normalize(g, 5, 16);
  uint64_t B = image::normalize(b, 5, 16);

  if(colorEmulation->value()) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(960, R), 10, 16);
    G = image::normalize(min(960, G), 10, 16);
    B = image::normalize(min(960, B), 10, 16);
  }

  return R << 32 | G << 16 | B << 0;
}
