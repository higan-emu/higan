//Mega CD

struct MCD : M68K, Thread {
  Memory::Readable<uint8> bios;  //BIOS ROM
  Memory::Writable<uint8> pram;  //program RAM
  Memory::Writable<uint8> wram;  //work RAM
  Memory::Writable<uint8> bram;  //backup RAM

  //mcd.cpp
  auto load() -> void;
  auto unload() -> void;

  auto main() -> void;
  auto step(uint clocks) -> void override;
  auto power(bool reset) -> void;

  //bus.cpp
  enum : uint { zero = 0 };
  auto read(uint1 size, uint24 address, uint16 data = 0) -> uint16 override;
  auto write(uint1 size, uint24 address, uint16 data) -> void override;

  //io.cpp
  auto readIO(uint1 size, uint24 address, uint16 data = 0) -> uint16;
  auto writeIO(uint1 size, uint24 address, uint16 data) -> void;

  auto readIO(uint24 address) -> uint8;
  auto writeIO(uint24 address, uint8 data) -> void;

  //external.cpp
  auto external_read(uint1 size, uint22 address, uint16 data = 0) -> uint16;
  auto external_write(uint1 size, uint22 address, uint16 data) -> void;

  auto external_readIO(uint1 size, uint24 address, uint16 data = 0) -> uint16;
  auto external_writeIO(uint1 size, uint24 address, uint16 data) -> void;

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

    uint16 counter;
    uint16 decoder;
  } io;

  struct LED {
    uint1 red;
    uint1 green;
  } led;

  struct IRQ {
    //irq.cpp
    auto raise() -> void;
    auto lower() -> void;
    auto acknowledge() -> bool;
    static auto synchronize() -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    uint1 enable;
    uint1 pending;
  };

  struct IRQs {
    uint1 pending;

    IRQ reset;
    IRQ graphics;
    IRQ external;
    IRQ subcode;
  } irq;

  struct Communcation {
    uint8 cfm;
    uint8 cfs;
    uint8 command[16];
    uint8 status [16];
  } communication;

  struct CDC {
    //cdc.cpp
    auto clock() -> void;
    auto read() -> uint8;
    auto write(uint8 data) -> void;
    auto power(bool reset) -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    IRQ irq;
    uint12 stopwatch;

     uint1 dsr;
     uint1 edt;
     uint4 address;
     uint3 destination;

     uint8 comin = 0xff;
     uint8 sbout = 0xff;
    uint16 dbc;
    uint16 dac;
    uint16 wa;
    uint16 pt;
     uint8 head[2][4];

    struct IFSTAT {
      uint1 sten = 1;
      uint1 dten = 1;
      uint1 stbsy = 1;
      uint1 dtbsy = 1;
      uint1 deci = 1;
      uint1 dtei = 1;
      uint1 cmdi = 1;
    } ifstat;

    struct STAT0 {
      uint1 uceblk;
      uint1 erablk;
      uint1 sblk;
      uint1 wshort;
      uint1 lblk;
      uint1 nosync;
      uint1 ilsync;
      uint1 crcok;
    } stat0;

    struct STAT1 {
      uint1 sh3era;
      uint1 sh2era;
      uint1 sh1era;
      uint1 sh0era;
      uint1 modera;
      uint1 blkera;
      uint1 secera;
      uint1 menera;
    } stat1;

    struct STAT2 {
      uint1 rform0;
      uint1 rform1;
      uint1 nocor;
      uint1 mode;
      uint1 rmod0;
      uint1 rmod1;
      uint1 rmod2;
      uint1 rmod3;
    } stat2;

    struct STAT3 {
      uint1 cblk;
      uint1 wlong;
      uint1 valst = 1;
    } stat3;

    struct IFCTRL {
      uint1 souten;
      uint1 douten;  //data output enable
      uint1 stwai;
      uint1 dtwai;
      uint1 cmdbk;
      uint1 decien;
      uint1 dteien;
      uint1 cmdien;
    } ifctrl;

    struct CTRL0 {
      uint1 prq;
      uint1 orq;
      uint1 wrrq;
      uint1 eramrq;
      uint1 autorq;
      uint1 e01rq;
      uint1 decen;
    } ctrl0;

    struct CTRL1 {
      uint1 shdren;
      uint1 mbckrq;
      uint1 formrq;
      uint1 modrq;
      uint1 cowren;
      uint1 dscren;
      uint1 syden;
      uint1 syen;
    } ctrl1;

    struct CTRL2 {
      uint1 stentrg;
      uint1 stenctl;
      uint1 eramsl;
    } ctrl2;
  } cdc;

  struct CDD {
    //cdd.cpp
    auto clock() -> void;
    auto write() -> void;
    auto checksum() -> void;
    auto power(bool reset) -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    IRQ irq;
    uint16 counter;

    struct Fader {
       uint1 spindleSpeed;  //0 = normal-speed, 1 = double-speed
       uint2 deemphasis;    //0 = off, 1 = 44.1khz, 2 = 32khz, 3 = 48khz
      uint11 volume;
    } fader;

    uint1 hock;

    uint4 status [10];
    uint4 command[10];
  } cdd;

  struct Timer {
    //timer.cpp
    auto clock() -> void;
    auto power(bool reset) -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    IRQ irq;
    uint8 counter;
  } timer;

  struct PCM {
    shared_pointer<Stream> stream;
    Memory::Writable<uint8> ram;

    //pcm.cpp
    auto clock() -> void;
    auto read(uint13 address, uint8 data) -> uint8;
    auto write(uint13 address, uint8 data) -> void;
    auto power(bool reset) -> void;

    //serialization.cpp
    auto serialize(serializer&) -> void;

    struct IO {
      uint1 enable;
      uint4 bank;
      uint3 channel;
    } io;

    struct Channel {
       uint1 enable;
       uint8 envelope;
       uint8 pan = 0xff;
      uint16 step;
      uint16 loop;
       uint8 start;
      uint32 address;
    } channels[8];
  } pcm;
};

extern MCD mcd;
