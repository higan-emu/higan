struct Home : Panel {
  Home(View*);
  auto show() -> void override;
  auto hide() -> void override;

  Frame frame{this, Size{~0, ~0}};
    Canvas iconCanvas{&frame};
  HorizontalLayout controlLayout{this, Size{~0, 0}};
    Button spacerButton{&controlLayout, Size{0, 0}};
};
