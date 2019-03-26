auto PSG::select(uint8 data) -> void {
  io.register = data;
}

//AY-3-8910: unused bits return 0
//YM2149: unused bits return values written to them
auto PSG::read() -> uint8 {
  uint8 data;

  switch(io.register) {
  case  0:
    data.bits(0,7) = toneA.period.bits(0, 7);
    break;
  case  1:
    data.bits(0,3) = toneA.period.bits(8,11);
  //data.bits(4,7) = toneA.unused;
    break;
  case  2:
    data.bits(0,7) = toneB.period.bits(0, 7);
    break;
  case  3:
    data.bits(0,3) = toneB.period.bits(8,11);
  //data.bits(4,7) = toneB.unused;
    break;
  case  4:
    data.bits(0,7) = toneC.period.bits(0, 7);
    break;
  case  5:
    data.bits(0,3) = toneC.period.bits(8,11);
  //data.bits(4,7) = toneC.unused;
    break;
  case  6:
    data.bits(0,4) = noise.period;
  //data.bits(5,7) = noise.unused;
    break;
  case  7:
    data.bit(0) = channelA.tone;
    data.bit(1) = channelB.tone;
    data.bit(2) = channelC.tone;
    data.bit(3) = channelA.noise;
    data.bit(4) = channelB.noise;
    data.bit(5) = channelC.noise;
    data.bit(6) = io.portA;
    data.bit(7) = io.portB;
    break;
  case  8:
    data.bits(0,3) = channelA.amplitude;
    data.bit (4)   = channelA.envelope;
  //data.bits(5,7) = channelA.unused;
    break;
  case  9:
    data.bits(0,3) = channelB.amplitude;
    data.bit (4)   = channelB.envelope;
  //data.bits(5,7) = channelB.unused;
    break;
  case 10:
    data.bits(0,3) = channelC.amplitude;
    data.bit (4)   = channelC.envelope;
  //data.bits(5,7) = channelC.unused;
    break;
  case 11:
    data.bits(0,7) = envelope.period.bits(0, 7);
    break;
  case 12:
    data.bits(0,7) = envelope.period.bits(8,15);
    break;
  case 13:
    data.bit (0)   = envelope.hold;
    data.bit (1)   = envelope.alternate;
    data.bit (2)   = envelope.attack;
    data.bit (3)   = envelope.repeat;
  //data.bits(4,7) = envelope.unused;
  case 14: return 0xff;  // I/O port A
  case 15: return 0xff;  // I/O port B
  }

  return data;
}

auto PSG::write(uint8 data) -> void {
  switch(io.register) {
  case  0:
    toneA.period.bits(0, 7) = data.bits(0,7);
    break;
  case  1:
    toneA.period.bits(8,11) = data.bits(0,3);
    toneA.unused.bits(0, 3) = data.bits(4,7);
    break;
  case  2:
    toneB.period.bits(0, 7) = data.bits(0,7);
    break;
  case  3:
    toneB.period.bits(8,11) = data.bits(0,3);
    toneB.unused.bits(0, 3) = data.bits(4,7);
    break;
  case  4:
    toneC.period.bits(0, 7) = data.bits(0,7);
    break;
  case  5:
    toneC.period.bits(8,11) = data.bits(0,3);
    toneC.unused.bits(0, 3) = data.bits(4,7);
    break;
  case  6:
    noise.period = data.bits(0,4);
    noise.unused = data.bits(5,7);
    break;
  case  7:
    channelA.tone  = data.bit(0);
    channelB.tone  = data.bit(1);
    channelC.tone  = data.bit(2);
    channelA.noise = data.bit(3);
    channelB.noise = data.bit(4);
    channelC.noise = data.bit(5);
    io.portA       = data.bit(6);
    io.portB       = data.bit(7);
    break;
  case  8:
    channelA.amplitude = data.bits(0,3);
    channelA.envelope  = data.bit (4);
    channelA.unused    = data.bits(5,7);
    break;
  case  9:
    channelB.amplitude = data.bits(0,3);
    channelB.envelope  = data.bit (4);
    channelB.unused    = data.bits(5,7);
    break;
  case 10:
    channelC.amplitude = data.bits(0,3);
    channelC.envelope  = data.bit (4);
    channelC.unused    = data.bits(5,7);
    break;
  case 11:
    envelope.period.bits(0, 7) = data.bits(0,7);
    break;
  case 12:
    envelope.period.bits(8,15) = data.bits(0,7);
    break;
  case 13:
    envelope.hold      = data.bit (0);
    envelope.alternate = data.bit (1);
    envelope.attack    = data.bit (2);
    envelope.repeat    = data.bit (3);
    envelope.unused    = data.bits(4,7);
    envelope.reload();
    break;
  case 14:  // I/O port A
    break;
  case 15:  // I/O port B
    break;
  }
}
