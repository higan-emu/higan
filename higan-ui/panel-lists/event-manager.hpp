struct EventManager : PanelList {
  EventManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void override;
  auto refresh(higan::Node::Object node) -> void;

  auto onChange() -> void;
  auto onToggle(ListViewItem) -> void;

  ListView listView{this, Size{~0, ~0}};
};
