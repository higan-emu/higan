struct CPU : TLCS900H, Thread {
  Memory::Writable<uint8> ram;

  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power() -> void;

  auto setInterruptAPU(boolean line) -> void;
  auto setInterruptHblank(boolean line) -> void;
  auto setInterruptVblank(boolean line) -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //io.cpp
  auto readIO(uint8 address) -> uint8;
  auto writeIO(uint8 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  struct IO {
    struct IRQ {
      boolean apu;
      boolean hblank;
      boolean vblank;
    } irq;

    uint8 apuPort;
  } io;
};

extern CPU cpu;
