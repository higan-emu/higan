Hacks hacks;

auto Hacks::load(Node::Object parent, Node::Object from) -> void {
  parent->append(node = Node::Settings::create("Hacks"));
  from = Node::load(node, from);

  node->append(ppu.fast = Node::Boolean::create("Fast PPU", false));
  Node::load(ppu.fast, from);

  node->append(ppu.noSpriteLimit = Node::Boolean::create("No Sprite Limit", false));
  Node::load(ppu.noSpriteLimit, from);

  node->append(ppu.hiresMode7 = Node::Boolean::create("Hires Mode 7", false));
  Node::load(ppu.hiresMode7, from);

  node->append(dsp.fast = Node::Boolean::create("Fast DSP", false));
  Node::load(dsp.fast, from);

  node->append(coprocessors.fast = Node::Boolean::create("Fast Coprocessors", false));
  Node::load(coprocessors.fast, from);
}
