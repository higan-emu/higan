struct CPU : Z80, Z80::Bus, Thread {
  Node::Component node;
  Memory::Writable<uint8> ram;

  struct Debugger {
    //debugger.cpp
    auto load(Node::Object) -> void;
    auto instruction() -> void;
    auto interrupt(string_view) -> void;

    struct Memory {
      Node::Memory ram;
    } memory;

    struct Tracer {
      Node::Instruction instruction;
      Node::Notification interrupt;
    } tracer;
  } debugger;

  auto synchronizing() const -> bool override { return scheduler.synchronizing(); }

  //cpu.cpp
  auto load(Node::Object) -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void override;

  auto power() -> void;
  auto setIrq(bool line) -> void;

  //memory.cpp
  auto read(uint16 address) -> uint8 override;
  auto write(uint16 address, uint8 data) -> void override;
  auto readBanked(uint3 bank, uint16 address) -> uint8;
  auto writeBanked(uint3 bank, uint16 address, uint8 data) -> void;

  auto in(uint16 address) -> uint8 override;
  auto out(uint16 address, uint8 data) -> void override;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  uint1 irqLine;
};

extern CPU cpu;
