//Famicom Disk System

struct FDS : Thread {
  Node::Port port;
  Node::Peripheral node;
  uint1 present;

  Memory::Writable<uint8> sideA;
  Memory::Writable<uint8> sideB;

  inline auto mirroring() const -> bool { return io.mirroring; }

  //fds.cpp
  auto load(Node::Object, Node::Object) -> void;
  auto unload() -> void;
  auto connect(Node::Peripheral) -> void;
  auto disconnect() -> void;
  auto save() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //io.cpp
  auto read(uint16 address) -> uint8;
  auto write(uint16 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  struct Information {
    string metadata;
  } information;

  struct IRQ {
    uint16 counter;
    uint16 period;
     uint1 repeat;
     uint1 onTimer;
     uint1 onTransfer;
     uint1 triggered;
  } irq;

  struct IO {
    uint1 enableDisk;
    uint1 enableAudio;
    uint1 enableMotor;
    uint1 accessMode;     //0 = write, 1 = read
    uint1 mirroring = 1;  //0 = vertical, 1 = horizontal
    uint1 crcControl;
  } io;
};

extern FDS fds;
