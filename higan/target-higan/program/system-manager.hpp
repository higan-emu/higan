struct SystemManager : Panel {
  SystemManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto deselect() -> void;

  auto eventActivate() -> void;
  auto eventChange() -> void;
  auto eventContext() -> void;
  auto eventCreate() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;

  ListView systemList{this, Size{~0, ~0}};

  PopupMenu systemListMenu;
    MenuItem systemListCreate{&systemListMenu};
    MenuItem systemListRename{&systemListMenu};
    MenuItem systemListRemove{&systemListMenu};
};
