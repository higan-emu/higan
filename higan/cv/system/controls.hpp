struct Controls {
  Node::Button reset;

  auto load(Node::Object, Node::Object) -> void;
  auto poll() -> void;
};

extern Controls controls;
