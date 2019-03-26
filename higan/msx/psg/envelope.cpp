auto PSG::Envelope::clock() -> void {
  if(holding) return;
  if(++counter < period) return;

  counter = 0;
  if(!attacking) {
    if(phase !=  0) return (void)phase--;
  } else {
    if(phase != 15) return (void)phase++;
  }

  if(!repeat) {
    phase = 0;
    holding = 1;
  } else if(hold) {
    if(alternate) phase ^= 15;
    holding = 1;
  } else if(alternate) {
    attacking ^= 1;
  } else {
    phase = !attacking ? 15 : 0;
  }
}

auto PSG::Envelope::reload() -> void {
  holding = 0;
  attacking = attack;
  phase = !attacking ? 15 : 0;
}
