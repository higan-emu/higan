auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);

  node->type   = "LCD";
  node->width  = 224 + (Model::WonderSwanColor() || Model::SwanCrystal() || Model::MamaMitte() ? 13 : 0);
  node->height = 144 + (Model::WonderSwan() ? 13 : 0);
  node->scaleX = 1.0;
  node->scaleY = 1.0;
  node->colors = 1 << 12;
  node->color  = [&](auto index) { return color(index); };

  colorEmulation = Node::append<Node::Boolean>(parent, from, "Color Emulation", true, [&](auto value) {
    ppu.screen.setPalette();
  });
  colorEmulation->dynamic = true;

  interframeBlending = Node::append<Node::Boolean>(parent, from, "Interframe Blending", true, [&](auto value) {
    ppu.screen.setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;

  if(!Model::PocketChallengeV2()) {
    orientation = Node::append<Node::String>(parent, from, "Orientation", "Automatic", [&](auto value) {
      ppu.updateOrientation();
    });
    orientation->dynamic = true;
    orientation->setAllowedValues({"Automatic", "Horizontal", "Vertical"});

    showIcons = Node::append<Node::Boolean>(parent, from, "Show Icons", true, [&](auto value) {
      ppu.updateIcons();
    });
    showIcons->dynamic = true;
  } else {
    //the Pocket Challenge V2 has no portrait-oriented software, and lacks an icon indicator strip.
    //create empty nodes, so that they are not null if accessed, but their values will not be used.
    orientation = Node::String();
    showIcons = Node::Boolean();
  }
}

auto System::Video::color(uint32 color) -> uint64 {
  uint b = color.bit(0, 3);
  uint g = color.bit(4, 7);
  uint r = color.bit(8,11);

  uint64_t R = image::normalize(r, 4, 16);
  uint64_t G = image::normalize(g, 4, 16);
  uint64_t B = image::normalize(b, 4, 16);

  if(colorEmulation->value()) {
    R = (r * 26 + g *  4 + b *  2);
    G = (         g * 24 + b *  8);
    B = (r *  6 + g *  4 + b * 22);
    R = image::normalize(min(480, R), 9, 16);
    G = image::normalize(min(480, G), 9, 16);
    B = image::normalize(min(480, B), 9, 16);
  }

  return R << 32 | G << 16 | B << 0;
}
