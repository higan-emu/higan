//TMP95C061

extern bool tracing;

struct CPU : TLCS900H, Thread {
  Memory::Writable<uint8> ram;

  //cpu.cpp
  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto pollPowerButton() -> void;
  auto power() -> void;

  auto load() -> void;
  auto save() -> void;
  auto unload() -> void;

  //memory.cpp
  auto read(uint24 address) -> uint8 override;
  auto write(uint24 address, uint8 data) -> void override;

  //io.cpp
  auto readIO(uint8 address) -> uint8;
  auto writeIO(uint8 address, uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint24 mar;  //memory address register
  uint16 mdr;  //memory data register

  //pin.cpp

  // PA0, /WAIT
  struct Pin32 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin32, &pa0 = pin32, &wait = pin32;

  // PA1, TI0, (/HBLANK)
  struct Pin33 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin33, &pa1 = pin33, &ti0 = pin33, &hblank = pin33;

  // PA2, TO1
  struct Pin34 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
    uint1 mode;
  } pin34, &pa2 = pin34, &to1 = pin34;

  // PA3, TO3
  struct Pin35 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
    uint1 mode;
  } pin35, &pa3 = pin35, &to3 = pin35;

  // PB0, INT4, TI4, (/VBLANK)
  struct Pin36 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin36, &pb0 = pin36, &ti4 = pin36, &vblank = pin36;

  // PB1, INT5, TI5, (APU)
  struct Pin37 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin37, &pb1 = pin37, &ti5 = pin37;

  // PB2, TO4
  struct Pin38 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
    uint1 mode;
  } pin38, &pb2 = pin38, &to4 = pin38;

  // PB3, TO5
  struct Pin39 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
    uint1 mode;
  } pin39, &pb3 = pin39, &to5 = pin39;

  // PB4, INT6, TI6
  struct Pin40 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin40, &pb4 = pin40, &ti6 = pin40;

  // PB5, INT7, TI7
  struct Pin41 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin41, &pb5 = pin41, &ti7 = pin41;

  // PB6, TO6
  struct Pin42 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
    uint1 mode;
  } pin42, &pb6 = pin42, &to6 = pin42;

  // PB7, INT0
  struct Pin43 {
    operator bool() const;
    auto operator=(bool) -> void;
    uint1 pin;
    uint1 flow;
  } pin43, &pb7 = pin43;

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

  struct RTC {
    //rtc.cpp
    auto step(uint clocks) -> void;
    auto daysInMonth() -> uint8;
    auto daysInFebruary() -> uint8;

    uint32 counter;
    uint1  enable;
    uint8  second;
    uint8  minute;
    uint8  hour;
    uint8  weekday;
    uint8  day;
    uint8  month;
    uint8  year;
  } rtc;

  struct DMA {
    uint8 vector;
  } dma0, dma1, dma2, dma3;

  struct ChipSelect {
    //memory.cpp
    auto select() -> void;

    uint2 wait;
    uint1 width;  //0 = 16-bit, 1 = 8-bit
  };

  struct ChipSelect0 : ChipSelect {
    uint1  enable;
    uint24 address;
    uint24 mask;
  } cs0;

  struct ChipSelect1 : ChipSelect {
    uint1  enable;
    uint24 address;
    uint24 mask;
  } cs1;

  struct ChipSelect2 : ChipSelect {
    uint1  enable;
    uint24 address;
    uint24 mask;
    uint1  mode;  //0 = 000080-ffffff, 1 = address,mask
  } cs2;

  struct ChipSelect3 : ChipSelect {
    uint1  enable;
    uint24 address;
    uint24 mask;
    uint1  cas;  //0 = /CS3, 1 = /CAS
  } cs3;

  struct ChipSelectExternal : ChipSelect {
  } csx;

  struct Clock {
    //0 = 6144000hz
    //1 = 3072000hz
    //2 = 1536000hz
    //3 =  768000hz
    //4 =  384000hz
    //5 =  192000hz? (undocumented)
    //6 =   96000hz? (undocumented)
    //7 =   48000hz? (undocumented)
    uint3 rate;
  } clock;

  struct IO {
    uint1 rtsDisable;
    uint8 apuPort;
  } io;
};

extern CPU cpu;
