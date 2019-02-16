struct Display {
  Node::Video node;
  Node::Boolean interframeBlending;
  shared_pointer<Screen> screen;

  auto load(Node::Object, Node::Object) -> void;
  auto color(uint32) -> uint64;
};

extern Display display;
