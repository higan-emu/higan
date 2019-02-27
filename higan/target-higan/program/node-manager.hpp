struct NodeManager : Panel {
  NodeManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto bind(higan::Node::Object) -> void;
  auto refresh() -> void;
  auto refresh(higan::Node::Object, uint depth = 1) -> void;
  auto refreshSettings() -> void;
  auto deselect() -> void;

  auto eventChange() -> void;

  ListView nodeList{this, Size{~0, ~0}};

  higan::Node::Object root;
};
