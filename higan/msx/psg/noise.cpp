auto PSG::Noise::clock() -> void {
  if(++counter < period) return;

  counter = 0;
  if(phase ^= 1) lfsr = (lfsr.bit(0) ^ lfsr.bit(3)) << 16 | lfsr >> 1;
}
