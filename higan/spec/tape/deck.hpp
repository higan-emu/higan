struct TapeDeck {
  Node::Peripheral node;
  Node::Button playStop;
  TapeDeckTray tray;

  TapeDeck(string name);

  auto playing() -> bool { return state.playing; }
  auto read() -> uint1 { return state.output; }

  auto load(Node::Object) -> void;
  auto unload() -> void;
  auto power() -> void;

  const string name;

  struct {
    bool playing;
    uint1 output;
  } state;
};

extern TapeDeck tapeDeck;
