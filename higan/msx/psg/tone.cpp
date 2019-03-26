auto PSG::Tone::clock() -> void {
  if(++counter < period) return;

  counter = 0;
  phase ^= 1;
}
