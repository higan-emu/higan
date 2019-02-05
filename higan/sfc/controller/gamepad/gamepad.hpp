struct Gamepad : Controller {
  static auto create() -> Node::Peripheral;
  Node::Button up;
  Node::Button down;
  Node::Button left;
  Node::Button right;
  Node::Button b;
  Node::Button a;
  Node::Button y;
  Node::Button x;
  Node::Button l;
  Node::Button r;
  Node::Button select;
  Node::Button start;

  Gamepad(Node::Peripheral, uint port);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;
};
