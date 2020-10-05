struct Kempston : Expansion {
  Node::Button up;
  Node::Button down;
  Node::Button left;
  Node::Button right;
  Node::Button fire;

  Kempston(Node::Port parent);

  auto romcs() -> bool { return false; }
  auto mapped(uint16 address, bool io) -> bool { return io && (uint8)address == 0x1f; }

  auto read(uint16 address) -> uint8 { return 0xff; }
  auto write(uint16 address, uint8 data) -> void {}

  auto in(uint16 address) -> uint8;
  auto out(uint16 address, uint8 data) -> void {}
};
