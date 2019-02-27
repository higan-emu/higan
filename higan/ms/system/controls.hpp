struct Controls {
  //Master System
  Node::Button pause;
  Node::Button reset;

  //Game Gear
  Node::Button up;
  Node::Button down;
  Node::Button left;
  Node::Button right;
  Node::Button one;
  Node::Button two;
  Node::Button start;

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
