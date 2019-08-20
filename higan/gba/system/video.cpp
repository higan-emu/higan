auto System::Video::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Video>(parent, from, "Video");
  from = Node::scan(parent = node, from);
  node->type   = "CRT";
  node->width  = 240;
  node->height = 160;
  node->scaleX = 1.0;
  node->scaleY = 1.0;
  node->colors = 1 << 15;
  node->color  = [&](auto index) { return color(index); };

  colorEmulation = Node::append<Node::Boolean>(parent, from, "Color Emulation", true, [&](auto value) {
    ppu.display.setPalette();
  });
  colorEmulation->dynamic = true;

  interframeBlending = Node::append<Node::Boolean>(parent, from, "Interframe Blending", true, [&](auto value) {
    ppu.display.setInterframeBlending(value);
  });
  interframeBlending->dynamic = true;

  orientation = Node::append<Node::String>(parent, from, "Orientation", "Horizontal", [&](auto value) {
    ppu.display.setRotateLeft(value == "Vertical");
    node->rotation = (value == "Vertical" ? 90 : 0);
  });
  orientation->dynamic = true;
  orientation->setAllowedValues({"Horizontal", "Vertical"});
}

auto System::Video::color(uint32 color) -> uint64 {
  uint R = color.bit( 0, 4);
  uint G = color.bit( 5, 9);
  uint B = color.bit(10,14);

  uint64 r = image::normalize(R, 5, 16);
  uint64 g = image::normalize(G, 5, 16);
  uint64 b = image::normalize(B, 5, 16);

  if(colorEmulation->value()) {
    double lcdGamma = 4.0, outGamma = 2.2;
    double lb = pow(B / 31.0, lcdGamma);
    double lg = pow(G / 31.0, lcdGamma);
    double lr = pow(R / 31.0, lcdGamma);
    r = pow((  0 * lb +  50 * lg + 255 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    g = pow(( 30 * lb + 230 * lg +  10 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
    b = pow((220 * lb +  10 * lg +  50 * lr) / 255, 1 / outGamma) * (0xffff * 255 / 280);
  }

  return r << 32 | g << 16 | b << 0;
}
