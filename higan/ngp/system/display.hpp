struct Display {
  Node::Video node;
  Node::Boolean interframeBlending;
  shared_pointer<Screen> screen;

  auto load(Node::Object, Node::Object) -> void;
  auto colorNeoGeoPocket(uint32) -> uint64;
  auto colorNeoGeoPocketColor(uint32) -> uint64;
};

extern Display display;
