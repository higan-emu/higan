//Motorola 68000

struct CPU : M68K, Thread {
  enum class Interrupt : uint {
    Reset,
    HorizontalBlank,
    VerticalBlank,
  };

  using Thread::synchronize;

  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronize() -> void;

  auto raise(Interrupt) -> void;
  auto lower(Interrupt) -> void;

  auto power(bool reset) -> void;

  //bus.cpp
  enum : uint { zero = 0 };

  auto read(uint1 size, uint24 address, uint16 data = 0) -> uint16 override;
  auto write(uint1 size, uint24 address, uint16 data) -> void override;

  auto readIO(uint24 address) -> uint16;
  auto writeIO(uint24 address, uint16 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  Memory::Writable<uint8> ram;
  Memory::Readable<uint8> tmss;
  uint1 tmssEnable;

  struct IO {
    boolean version;  //0 = Model 1; 1 = Model 2+
    boolean romEnable;
    boolean vdpEnable[2];
  } io;

  struct State {
    uint32 interruptLine;
    uint32 interruptPending;
  } state;
};

extern CPU cpu;
