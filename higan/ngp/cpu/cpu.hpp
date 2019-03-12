//TMP95C061

extern bool tracing;

struct CPU : TLCS900H, Thread {
  Memory::Writable<uint8> ram;

  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto pollPowerButton() -> void;
  auto power() -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //io.cpp
  auto readIO(uint8 address) -> uint8;
  auto writeIO(uint8 address, uint8 data) -> void;

  //pin.cpp
  auto setPin33(bool) -> void;  //PA1, TI0
  auto setPin34(bool) -> void;  //PA2, TO1
  auto setPin35(bool) -> void;  //PA3, TO3
  auto setPin36(bool) -> void;  //PB0, TI4, INT4
  auto setPin37(bool) -> void;  //PB1, TI5, INT5
  auto setPin38(bool) -> void;  //PB2, TO4
  auto setPin39(bool) -> void;  //PB3, TO5
  auto setPin40(bool) -> void;  //PB4, TI6, INT6
  auto setPin41(bool) -> void;  //PB5, TI7, INT7
  auto setPin42(bool) -> void;  //PB6, TO6

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  struct Pins {
    uint1 p33;
    uint1 p34;
    uint1 p35;
    uint1 p36;
    uint1 p37;
    uint1 p38;
    uint1 p39;
    uint1 p40;
    uint1 p41;
    uint1 p42;
  } pins;

  struct Interrupt {
    //interrupt.cpp
    inline auto test(maybe<Interrupt&>&) const -> void;
    inline auto fired() -> void;
    auto set(bool line) -> void;
    auto clear() -> void;
    auto raise() -> void;
    auto lower() -> void;

    uint8 vector;
    uint1 enable = 1;
    uint1 maskable = 1;
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

  //non-maskable
  Interrupt nmi   {0x20};
  //maskable
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

  //8-bit timers

  struct TimerByte {
    //timers.cpp
    auto enableIf(bool) -> void;

    uint1 enable;
    uint2 mode;
    uint8 counter;
    uint8 compare;

    maybe<Interrupt&> interrupt;
  };

  struct TimerFlipFlop {
    //timers.cpp
    auto invert() -> void;
    auto raise() -> void;
    auto lower() -> void;

    uint1 enable;
    uint1 select;
    uint1 output;

    function<void (bool)> edge;
  };

  struct TimerPair {
    //timers.cpp
    auto clockExternal() -> void;
    auto clock0() -> void;
    auto clock1() -> void;
    auto clock2() -> void;
    auto clock3() -> void;

    auto clockLo() -> void;
    auto clockHi() -> void;
    auto clockPair() -> void;
    auto clockPPG() -> void;
    auto clockPWM() -> void;

    uint2 mode;
    uint2 pwm;
    TimerByte lo;
    TimerByte hi;
    TimerFlipFlop ff;

    struct Buffer {
      uint1 enable;
      uint8 compare;
    } buffer;
  };

  //16-bit timers

  struct TimerWord {
    //timers.cpp
    auto enableIf(bool) -> void;
    auto clock0() -> void;
    auto clock1() -> void;
    auto clock2() -> void;

    auto clock() -> void;

    uint1 enable;
    uint1 line;
    uint2 mode;
    uint1 clearOnMatch;
    uint2 captureMode;

    uint16 counter;
    uint16 compareA;
    uint16 compareB;
    uint16 captureA;
    uint16 captureB;

    struct FlipFlopA {
      uint1 flipOnMatchA;
      uint1 flipOnMatchB;
      uint1 flipOnLoadA;
      uint1 flipOnLoadB;
      uint1 output;
    } ffA;

    struct FlipFlopB {
      uint1 flipOnMatchB;
      uint1 flipOnLoadB;
      uint1 output;
    } ffB;

    struct Buffer {
      uint1  enable;
      uint16 compare;
    } buffer;

    maybe<Interrupt&> interruptA;
    maybe<Interrupt&> interruptB;

    function<void (bool)> outputA;
    function<void (bool)> outputB;
  };

  struct Timers {
    //timers.cpp
    auto step(uint clocks) -> void;

    auto input0(bool) -> void;
    auto input4(bool) -> void;
    auto input5(bool) -> void;
    auto input6(bool) -> void;
    auto input7(bool) -> void;

    uint1  enable;
    uint32 clock;

    TimerPair timer01;
    TimerPair timer23;
    TimerWord timer4;
    TimerWord timer5;
  } timers;

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

  struct Watchdog {
    uint1 drive;
    uint1 reset;
    uint2 standby;
    uint1 warmup;
    uint2 frequency;
    uint1 enable;
  } watchdog;

  struct IO {
    uint8 apuPort;
  } io;
};

extern CPU cpu;
