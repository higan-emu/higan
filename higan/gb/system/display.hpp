struct Display {
  Node::Video node;
  Node::Boolean colorEmulation;
  Node::Boolean interframeBlending;
  shared_pointer<Screen> screen;

  //display.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto colorGameBoy(uint32) -> uint64;
  auto colorGameBoyColor(uint32) -> uint64;
};

extern Display display;
