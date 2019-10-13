struct NodeManager : PanelList {
  NodeManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto refresh(higan::Node::Object node, uint depth) -> void;
  auto refreshSettings() -> void;
  auto name(higan::Node::Object node, uint depth) -> string;
  auto onChange() -> void;

  higan::Node::Object root;
  ListView listView{this, Size{~0, ~0}};
};
