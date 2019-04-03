struct CPU : Z80, Z80::Bus, Thread {
  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto synchronizing() const -> bool override;

  auto setNMI(bool value) -> void;
  auto setIRQ(bool value) -> void;

  auto power() -> void;

  //memory.cpp
  auto read(uint16 address) -> uint8 override;
  auto write(uint16 address, uint8 data) -> void override;

  auto in(uint16 address) -> uint8 override;
  auto out(uint16 address, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  vector<Thread*> peripherals;

private:
  Memory::Writable<uint8> ram;

  struct State {
    bool nmiLine = 0;
    bool irqLine = 0;
    bool enableFM = 0;
  } state;
};

extern CPU cpu;
