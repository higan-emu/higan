struct Display {
  Node::Video node;
  shared_pointer<Screen> screen;

  //display.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto color(uint32) -> uint64;
  auto colorMSX(uint32) -> uint64;
  auto colorMSX2(uint32) -> uint64;
};

extern Display display;
