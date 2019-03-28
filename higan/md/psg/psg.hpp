struct PSG : SN76489, Thread {
  shared_pointer<Stream> stream;

  auto main() -> void;
  auto step(uint clocks) -> void;

  auto power(bool reset) -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  double volume[16];
};

extern PSG psg;
