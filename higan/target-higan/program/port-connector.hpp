struct PortConnector : Panel {
  PortConnector(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh(higan::Node::Port port) -> void;

  auto eventActivate() -> void;
  auto eventChange() -> void;
  auto eventContext() -> void;
  auto eventBrowse() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;

  PopupMenu contextMenu;
    MenuItem renameAction{&contextMenu};
    MenuItem removeAction{&contextMenu};

  Label locationLabel{this, Size{~0, 0}};
  ListView peripheralList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button browseButton{&controlLayout, Size{80, 0}};
    Label nameLabel{&controlLayout, Size{0, 0}};
    LineEdit nameValue{&controlLayout, Size{~0, 0}};
    Button acceptButton{&controlLayout, Size{80, 0}};

  higan::Node::Port port;
};
