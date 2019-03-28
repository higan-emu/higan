struct PSG : AY38910, Thread {
  shared_pointer<Stream> stream;

  //psg.cpp
  auto main() -> void;
  auto step(uint clocks) -> void;
  auto power() -> void;

  //serialization.cpp
  auto serialize(serializer&) -> void;

private:
  double volume[16];
};

extern PSG psg;
