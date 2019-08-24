struct LogSettings : Panel {
  LogSettings(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto eventToggle(TableViewCell cell) -> void;

  VerticalLayout layout{this, Size{~0, ~0}};
    Label logHeader{&layout, Size{~0, 0}};
    TableView logList{&layout, Size{~0, ~0}};
};
