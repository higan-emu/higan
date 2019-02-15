struct Display {
  Node::Video node;
  shared_pointer<Screen> screen;

  //display.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto color(uint32) -> uint64;
};

extern Display display;
