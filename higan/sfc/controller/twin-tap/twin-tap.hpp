struct TwinTap : Controller {
  static auto create() -> Node::Peripheral;
  Node::Button one;
  Node::Button two;

  TwinTap(Node::Peripheral);

  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched = 0;
  uint counter = 0;
};
