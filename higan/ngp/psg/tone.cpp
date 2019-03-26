auto PSG::Tone::run() -> uint1 {
  if(!counter--) {
    counter = pitch;
    output ^= 1;
  }
  return output;
}
