struct Display {
  Node::Video node;
  shared_pointer<Screen> screen;

  auto load(Node::Object, Node::Object) -> void;
  auto color(uint32) -> uint64;
  auto colorMasterSystem(uint32) -> uint64;
  auto colorGameGear(uint32) -> uint64;
};

extern Display display;
