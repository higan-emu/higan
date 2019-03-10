//TMP95C061

struct CPU : TLCS900H, Thread {
  Memory::Writable<uint8> ram;

  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power() -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //io.cpp
  auto readIO(uint8 address) -> uint8;
  auto writeIO(uint8 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  struct Interrupt {
    //interrupt.cpp
    inline auto test(maybe<Interrupt&>&) const -> void;
    inline auto fired() -> void;
    auto set(bool line) -> void;
    auto clear() -> void;
    auto raise() -> void;
    auto lower() -> void;

    uint8 vector;
    uint3 priority;
    uint1 line;
    uint1 request;

    struct Level {
      uint1 high;
      uint1 low;
    } level;

    struct Edge {
      uint1 rising;
      uint1 falling;
    } edge;
  };

  Interrupt int0  {0x28};
  Interrupt intad {0x70};
  Interrupt int4  {0x2c};
  Interrupt int5  {0x30};
  Interrupt int6  {0x34};
  Interrupt int7  {0x38};
  Interrupt intt0 {0x40};
  Interrupt intt1 {0x44};
  Interrupt intt2 {0x48};
  Interrupt intt3 {0x4c};
  Interrupt inttr4{0x50};
  Interrupt inttr5{0x54};
  Interrupt inttr6{0x58};
  Interrupt inttr7{0x5c};
  Interrupt intrx0{0x60};
  Interrupt inttx0{0x64};
  Interrupt intrx1{0x68};
  Interrupt inttx1{0x6c};
  Interrupt inttc0{0x74};
  Interrupt inttc1{0x78};
  Interrupt inttc2{0x7c};
  Interrupt inttc3{0x80};

  struct ADC {
    //adc.cpp
    auto step(uint clocks) -> void;

    uint2 channel;
    uint1 speed;  //0 = 160 states, 1 = 320 states
    uint1 scan;
    uint1 repeat;
    uint1 busy;
    uint1 end;

    uint10 result[4];

    integer counter;
  } adc;

  struct IO {
    uint8 apuPort;
  } io;
};

extern CPU cpu;
