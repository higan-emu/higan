struct VDP : TMS9918, V9938, Thread {
  Shared::Screen display;

  //vdp.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;

  auto step(uint clocks) -> void override;
  auto irq(bool line) -> void override;
  auto frame() -> void override;
  auto refresh() -> void;
  auto power() -> void;

  auto read(uint2 port) -> uint8;
  auto write(uint2 port, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;
};

extern VDP vdp;
