auto SN76489::Noise::run() -> void {
  if(counter--) return;

  counter = array<uint10[4]>{0x10, 0x20, 0x40, pitch}[rate];
  if(clock ^= 1) {  //0->1 transition
    output = !lfsr.bit(0);
    lfsr = (lfsr.bit(0) ^ lfsr.bit(3) & enable) << 15 | lfsr >> 1;
  }
}

auto SN76489::Noise::power() -> void {
  volume = ~0;
  counter = 0;
  pitch = 0;
  enable = 0;
  rate = 0;
  lfsr = 0x8000;
  clock = 0;
  output = 0;
}
