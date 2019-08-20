auto System::Audio::load(Node::Object parent, Node::Object from) -> void {
  node = Node::append<Node::Audio>(parent, from, "Audio");
  from = Node::scan(parent = node, from);

  headphones = Node::append<Node::Boolean>(parent, from, "Headphones", true, [&](auto value) {
    apu.r.headphonesConnected = value;
    ppu.updateIcons();
  });
  headphones->dynamic = true;
}
