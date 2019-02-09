struct Justifier : Controller {
  static auto create() -> Node::Peripheral;
  ControllerPort linkPort;
  Node::Axis x;
  Node::Axis y;
  Node::Button trigger;
  Node::Button start;

  shared_pointer<Sprite> sprite;

  Justifier(Node::Peripheral);
  ~Justifier();

  auto main() -> void;
  auto data() -> uint2;
  auto latch(bool data) -> void;

  auto linked() -> maybe<Justifier&>;

//private:
  bool latched;
  uint counter;
  uint previous;

  bool active;
  int  cx;
  int  cy;
};
