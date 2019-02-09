struct SystemManager : Panel {
  SystemManager(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto scan(string location, uint depth = 0) -> void;

  auto eventActivate() -> void;
  auto eventChange() -> void;
  auto eventCreate() -> void;
  auto eventRename() -> void;
  auto eventRemove() -> void;

  ListView systemList{this, Size{~0, ~0}};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button createButton{&controlLayout, Size{80, 0}};
    Button renameButton{&controlLayout, Size{80, 0}};
    Button removeButton{&controlLayout, Size{80, 0}};
};
