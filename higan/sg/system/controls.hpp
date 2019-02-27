struct Controls {
  Node::Button pause;

  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;
};

extern Controls controls;
