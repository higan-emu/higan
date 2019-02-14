struct Display {
  Node::Video node;
  Node::Boolean colorEmulation;
  Node::Boolean interframeBlending;
  Node::String orientation;
  shared_pointer<Screen> screen;

  auto load(Node::Object, Node::Object) -> void;
  auto color(uint32) -> uint64;
};

extern Display display;
