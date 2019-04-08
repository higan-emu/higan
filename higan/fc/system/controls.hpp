struct Controls {
  Node::Button reset;
  Node::Button microphone;

  //controls.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;
};

extern Controls controls;
