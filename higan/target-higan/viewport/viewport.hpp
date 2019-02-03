struct ViewportWindow : Window {
  higan::Node::Port::Video node;
  Video video;

  auto create(higan::Node::Port::Video) -> void;
  auto show(Window& parent) -> void;

  VerticalLayout layout{this};
    Viewport viewport{&layout, Size{~0, ~0}};
};
