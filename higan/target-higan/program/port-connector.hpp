struct PortConnector : PanelItem {
  PortConnector(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto isIcarusType() const -> bool;
  auto refresh(higan::Node::Port port) -> void;

  auto eventImport() -> void;
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
    Button importButton{&controlLayout, Size{80_sx, 0}};
    Widget controlSpacer{&controlLayout, Size{~0, 0}};
    Button acceptButton{&controlLayout, Size{80_sx, 0}};

  higan::Node::Port port;
};
