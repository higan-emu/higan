struct Hacks {
  Node::Settings node;
  struct PPU {
    Node::Boolean fast;
    Node::Boolean noSpriteLimit;
    Node::Boolean hiresMode7;
  } ppu;
  struct DSP {
    Node::Boolean fast;
  } dsp;
  struct Coprocessors {
    Node::Boolean fast;
  } coprocessors;

  auto load(Node::Object, Node::Object) -> void;
};

extern Hacks hacks;
