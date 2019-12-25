struct SystemOverview : PanelItem {
  SystemOverview(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void override;
  auto scan(higan::Node::Object node, uint depth = 1) -> void;

  Label header{this, Size{~0, 0}};
  ListView nodeList{this, Size{~0, ~0}};
};
