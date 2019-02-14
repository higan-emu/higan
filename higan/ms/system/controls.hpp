struct Controls {
  //SG-1000, SC-3000, Master System
  Node::Button pause;

  //Master System
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
};

extern Controls controls;
