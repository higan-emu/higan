//T6W28 (stereo derivative of the SN76489)

struct PSG : Thread {
  shared_pointer<Stream> stream;

  //psg.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

  uint1   psgEnable;
  SN76489 psgLeft;
  SN76489 psgRight;

  uint8 dacLeft;
  uint8 dacRight;
};

extern PSG psg;
