struct ViewportWindow : Window {
  higan::Node::Video node;
  Video video;

  auto create(higan::Node::Video) -> void;
  auto show(Window parent) -> void;
  auto hide() -> void;

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};
};
