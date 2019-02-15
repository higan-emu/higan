struct Controls {
  Node::Button y1;
  Node::Button y2;
  Node::Button y3;
  Node::Button y4;
  Node::Button x1;
  Node::Button x2;
  Node::Button x3;
  Node::Button x4;
  Node::Button b;
  Node::Button a;
  Node::Button start;

  //controls.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;
};

extern Controls controls;
