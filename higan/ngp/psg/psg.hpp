//T6W28 (stereo derivative of the SN76489)

struct PSG : Thread {
  shared_pointer<Stream> stream;

  //psg.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;
  auto enable() -> void;
  auto disable() -> void;

  //io.cpp
  auto writeLeft(uint8 data) -> void;
  auto writeRight(uint8 data) -> void;

  auto writeLeftDAC(uint8 data) -> void;
  auto writeRightDAC(uint8 data) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  struct Tone {
    //tone.cpp
    auto run() -> void;

    uint10 counter;
    uint10 pitch;
    uint1  output;
    struct Volume {
      uint4 left  = 15;
      uint4 right = 15;
    } volume;
  } tone0, tone1, tone2;

  struct Noise {
    //noise.cpp
    auto run() -> void;

    uint10 counter;
    uint10 pitch;
    uint1  enable;
    uint2  rate;
    uint15 lfsr = 0x4000;
    uint1  clock;
    uint1  output;
    struct Volume {
      uint4 left  = 15;
      uint4 right = 15;
    } volume;
  } noise;

  struct {
    uint1 enable;
  } psg;

  struct DAC {
    uint8 left;
    uint8 right;
  } dac;

  uint3 select;
  int16 amplitude[16];
};

extern PSG psg;
