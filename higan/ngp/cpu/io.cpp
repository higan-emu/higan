auto CPU::readIO(uint8 address) -> uint8 {
  uint8 data = 0xff;

  switch(address) {
  case 0xb0:
    controls.poll();
    data.bit(0) = controls.upLatch;
    data.bit(1) = controls.downLatch;
    data.bit(2) = controls.leftLatch;
    data.bit(3) = controls.rightLatch;
    data.bit(4) = controls.a->value;
    data.bit(5) = controls.b->value;
    data.bit(6) = controls.option->value;
    data.bit(7) = 0;  //unused?
    break;

  case 0xb1:
    data.bit(0) = 0;  //power button
    data.bit(1) = 1;  //sub battery (CR2032)
    break;

  case 0xbc:
    data = io.apuPort;
    break;
  }

  return data;
}

auto CPU::writeIO(uint8 address, uint8 data) -> void {
  switch(address) {
  case 0xb9:
    if(data == 0x55) {
      apu.enable();
    }
    if(data == 0xaa) {
      apu.disable();
    }
    break;

  case 0xbc:
    io.apuPort = data;
    break;
  }
}
