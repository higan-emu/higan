struct Controls {
  Node::Button up;
  Node::Button down;
  Node::Button left;
  Node::Button right;
  Node::Button b;
  Node::Button a;
  Node::Button select;
  Node::Button start;

  //controls.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;

  bool yHold = 0;
  bool upLatch = 0;
  bool downLatch = 0;
  bool xHold = 0;
  bool leftLatch = 0;
  bool rightLatch = 0;
};

extern Controls controls;
