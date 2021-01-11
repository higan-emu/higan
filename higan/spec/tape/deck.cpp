TapeDeck tapeDeck{"Tape Deck"};

TapeDeck::TapeDeck(string name) : name(name) {
}

auto TapeDeck::load(Node::Object parent) -> void {
  node = parent->append<Node::Peripheral>(name);

  playStop = node->append<Node::Button>("Play/Stop");
  tray.load(node);
}

auto TapeDeck::power() -> void {
  state = {};
}

auto TapeDeck::unload() -> void {
  tray.unload();
  node = {};
}
