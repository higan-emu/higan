//General Instrument AY-3-8910
//Yamaha YM2149

struct PSG : Thread {
  shared_pointer<Stream> stream;

  //psg.cpp
  auto main() -> void;
  auto mix() -> double;
  auto step(uint clocks) -> void;

  auto power() -> void;

  //io.cpp
  auto select(uint8 data) -> void;
  auto read() -> uint8;
  auto write(uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

//private:
  struct Tone {
    //tone.cpp
    auto clock() -> void;

    uint12 period;
    uint12 counter;
    uint1  phase;
    uint4  unused;  //YM2149 only
  };

  struct Noise {
    //noise.cpp
    auto clock() -> void;

     uint5 period;
     uint5 counter;
     uint1 phase;
    uint17 lfsr = 1;
     uint3 unused;  //YM2149 only
  };

  struct Channel {
    uint1 tone;   //0 = enable, 1 = disable
    uint1 noise;  //0 = enable, 1 = disable
    uint4 amplitude;
    uint1 envelope;
    uint3 unused;  //YM2149 only
  };

  struct Envelope {
    //envelope.cpp
    auto clock() -> void;
    auto reload() -> void;

    uint16 period;
    uint16 counter;
    uint1  holding;
    uint1  attacking;
    uint4  phase;
    uint1  hold;
    uint1  alternate;
    uint1  attack;
    uint1  repeat;
    uint4  unused;  //YM2149 only
  };

  struct IO {
    uint4 register;
    uint1 portA;  //0 = input, 1 = output
    uint1 portB;  //0 = input, 1 = output
  };

  Tone toneA;
  Tone toneB;
  Tone toneC;
  Noise noise;
  Channel channelA;
  Channel channelB;
  Channel channelC;
  Envelope envelope;
  IO io;
  double amplitudes[16];
};

extern PSG psg;
