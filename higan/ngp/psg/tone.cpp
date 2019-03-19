auto PSG::Tone::run() -> void {
  if(counter--) return;

  counter = pitch;
  output ^= 1;
}
