auto SN76489::Tone::run() -> void {
  if(counter--) return;

  counter = pitch;
  output ^= 1;
}

auto SN76489::Tone::power() -> void {
  volume = ~0;
  counter = 0;
  pitch = 0;
  output = 0;
}
