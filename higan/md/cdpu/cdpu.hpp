//Mega CD - CPU

struct CDPU : M68K, M68K::Bus, Thread {
  Memory::Readable<uint8> bios;  //BIOS ROM
  Memory::Writable<uint8> pram;  //program RAM
  Memory::Writable<uint8> wram;  //work RAM
  Memory::Writable<uint8> bram;  //backup RAM

  //cdpu.cpp
  auto load() -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power(bool reset) -> void;

  //bus.cpp
  auto readByte(uint24 address) -> uint16 override;
  auto readWord(uint24 address) -> uint16 override;
  auto writeByte(uint24 address, uint16 data) -> void override;
  auto writeWord(uint24 address, uint16 data) -> void override;

  //io.cpp
  auto readIO(uint24 address) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  //external.cpp
  auto external_read(uint22 address) -> uint8;
  auto external_write(uint22 address, uint8 data) -> void;

  auto external_readIO(uint24 address) -> uint8;
  auto external_writeIO(uint24 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct IO {
    uint1 run;
    uint1 request;
    uint1 halt = 1;

    uint1 wramMode;  //0 = 2mbit mode, 1 = 1mbit mode
    uint2 pramBank;
    uint8 pramProtect;

    uint8 cfm;
    uint8 cfs;
    uint8 command[16];
    uint8 status[16];
    uint8 receive[10];
    uint8 transfer[10];

    uint16 counter;
    uint12 stopwatch;
     uint8 timer;
  } io;

  struct LED {
    uint1 red;
    uint1 green;
  } led;

  struct IRQ {
    uint1 enable;
    uint1 pending;
  };

  struct IRQs {
    auto synchronize() -> void;

    uint1 pending;

    IRQ reset;
    IRQ graphics;
    IRQ external;
    IRQ timer;
    IRQ cdd;
    IRQ cdc;
    IRQ subcode;
  } irq;
};

extern CDPU cdpu;
