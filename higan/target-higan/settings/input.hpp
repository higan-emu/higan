struct InputSettings : Panel {
  InputSettings(View*);
  auto show() -> void override;
  auto hide() -> void override;
  auto refresh() -> void;
  auto eventActivate() -> void;

  Frame frame{this, Size{~0, ~0}};
  VerticalLayout layout{&frame};

  Label driverHeader{&layout, Size{~0, 0}};
  HorizontalLayout driverLayout{&layout, Size{~0, 0}};
    Label driverLabel{&driverLayout, Size{0, 0}};
    ComboButton driverOption{&driverLayout, Size{0, 0}};
    Button activateButton{&driverLayout, Size{0, 0}};

  Label optionsHeader{&layout, Size{~0, 0}};
  HorizontalLayout optionsLayout{&layout, Size{~0, 0}};
    Label focusLabel{&optionsLayout, Size{0, 0}};
    RadioLabel focusPause{&optionsLayout, Size{0, 0}};
    RadioLabel focusBlock{&optionsLayout, Size{0, 0}};
    RadioLabel focusAllow{&optionsLayout, Size{0, 0}};
    Group focusGroup{&focusPause, &focusBlock, &focusAllow};
};
