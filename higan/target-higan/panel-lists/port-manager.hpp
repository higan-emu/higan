struct PortManager : PanelList {
  PortManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto refresh(higan::Node::Object node) -> void;

  auto onChange() -> void;

  higan::Node::Object root;
  ListView listView{this, Size{~0, ~0}};
};
