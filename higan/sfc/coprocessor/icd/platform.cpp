auto ICD::audio(const double* samples, uint channels) -> void {
  stream->write(samples);
}

auto ICD::input() -> uint8 {
  uint8 data = 0x00;
  switch(joypID) {
  case 0: data = ~r6004; break;
  case 1: data = ~r6005; break;
  case 2: data = ~r6006; break;
  case 3: data = ~r6007; break;
  }
  return data;
}
