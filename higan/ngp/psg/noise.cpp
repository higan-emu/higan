auto PSG::Noise::run() -> uint1 {
  if(!counter--) {
    counter = array<uint10[4]>{0x10, 0x20, 0x40, pitch}[rate];
    if(clock ^= 1) {  //0->1 transition
      output = !lfsr.bit(0);
      lfsr = (lfsr.bit(0) ^ lfsr.bit(2) & enable) << 14 | lfsr >> 1;
    }
  }
  return output;
}
