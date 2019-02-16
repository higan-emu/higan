struct Controls {
  Node::Button up;
  Node::Button down;
  Node::Button left;
  Node::Button right;
  Node::Button a;
  Node::Button b;
  Node::Button option;

  //controls.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;

private:
  bool yHold = 0;
  bool upLatch = 0;
  bool downLatch = 0;
  bool xHold = 0;
  bool leftLatch = 0;
  bool rightLatch = 0;
};

extern Controls controls;
