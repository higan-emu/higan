struct Gamepad : Controller {
  Node::Input::Button up;
  Node::Input::Button down;
  Node::Input::Button left;
  Node::Input::Button right;
  Node::Input::Button b;
  Node::Input::Button a;
  Node::Input::Button y;
  Node::Input::Button x;
  Node::Input::Button l;
  Node::Input::Button r;
  Node::Input::Button select;
  Node::Input::Button start;

  Gamepad(uint port, Node);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;
};
