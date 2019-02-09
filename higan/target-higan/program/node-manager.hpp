struct NodeManager : Panel {
  NodeManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto scan(higan::Node::Object node, uint depth = 1) -> void;

  auto eventChange() -> void;

  ListView nodeList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button spacerButton{&controlLayout, Size{0, 0}};

  higan::Node::Object root;
  higan::Node::Port port;
};
