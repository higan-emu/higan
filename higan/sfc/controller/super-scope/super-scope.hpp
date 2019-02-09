struct SuperScope : Controller {
  static auto create() -> Node::Peripheral;
  Node::Axis x;
  Node::Axis y;
  Node::Button trigger;
  Node::Button cursor;
  Node::Button turbo;
  Node::Button pause;

  shared_pointer<Sprite> sprite;

  SuperScope(Node::Peripheral);
  ~SuperScope();

  auto main() -> void;
  auto data() -> uint2;
  auto latch(bool data) -> void;

private:
  bool latched;
  uint counter;

  int  cx;
  int  cy;
  bool triggerValue;
  bool turboEdge;
  bool pauseEdge;

  bool offscreen;
  bool turboOld;
  bool triggerLock;
  bool pauseLock;
  uint previous;
};
