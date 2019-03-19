auto PSG::writeLeft(uint8 data) -> void {
  bool l = data.bit(7);
  if(l) select = data.bits(4,6);

  if(select.bit(0)) switch(select.bits(1,2)) {
  case 0: tone0.volume.left = data.bits(0,3); break;
  case 1: tone1.volume.left = data.bits(0,3); break;
  case 2: tone2.volume.left = data.bits(0,3); break;
  case 3: noise.volume.left = data.bits(0,3); break;
  }

  else switch(select.bits(1,2)) {
  case 0:
    if(l) tone0.pitch.bits(0,3) = data.bits(0,3);
    else  tone0.pitch.bits(4,9) = data.bits(0,5);
    break;
  case 1:
    if(l) tone1.pitch.bits(0,3) = data.bits(0,3);
    else  tone1.pitch.bits(4,9) = data.bits(0,5);
    break;
  case 2:
    if(l) tone2.pitch.bits(0,3) = data.bits(0,3);
    else  tone2.pitch.bits(4,9) = data.bits(0,5);
    break;
  }
}

auto PSG::writeRight(uint8 data) -> void {
  bool l = data.bit(7);
  if(l) select = data.bits(4,6);

  if(select.bit(0)) switch(select.bits(1,2)) {
  case 0: tone0.volume.right = data.bits(0,3); break;
  case 1: tone1.volume.right = data.bits(0,3); break;
  case 2: tone2.volume.right = data.bits(0,3); break;
  case 3: noise.volume.right = data.bits(0,3); break;
  }

  else switch(select.bits(1,2)) {
  case 2:
    if(l) noise.pitch.bits(0,3) = data.bits(0,3);
    else  noise.pitch.bits(4,9) = data.bits(0,5);
    break;
  case 3:
    noise.rate   = data.bits(0,1);
    noise.enable = data.bit(2);
    noise.lfsr   = 0x4000;
    break;
  }
}

auto PSG::writeLeftDAC(uint8 data) -> void {
  dac.left  = data;
}

auto PSG::writeRightDAC(uint8 data) -> void {
  dac.right = data;
}
