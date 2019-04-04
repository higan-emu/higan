auto FDS::read(uint16 address) -> uint8 {
  uint8 data = 0x00;

  switch(0x4030 | (uint4)address) {

  case 0x4030:
    data.bit(0) = irq.triggered;
    irq.triggered = 0;
    cpu.irqLine(0);
    return data;

  case 0x4032:
    data.bit(0) = !(bool)sideA;  //0 = disk inserted, 1 = disk not inserted
    data.bit(1) = !(bool)sideA;  //0 = disk ready, 1 = disk not ready
    data.bit(2) = 0;             //0 = not write protected, 1 = write protected
    return data;

  case 0x4033:
    data.bit(7) = 0;  //0 = battery good, 1 = battery voltage low
    return data;

  }

  return data;
}

auto FDS::write(uint16 address, uint8 data) -> void {
  switch(0x4020 | (uint4)address) {

  case 0x4020:
    irq.period.byte(0) = data;
    return;

  case 0x4021:
    irq.period.byte(1) = data;
    return;

  case 0x4022:
    if(!io.enableDisk) return;
    irq.repeat = data.bit(0);
    irq.onTimer = data.bit(1);
    if(irq.onTimer) {
      irq.counter = irq.period;
    } else {
      irq.triggered = 0;
      cpu.irqLine(0);
    }
    return;

  case 0x4023:
    io.enableDisk  = data.bit(0);
    io.enableAudio = data.bit(1);
    if(!io.enableDisk) {
      irq.triggered = 0;
      cpu.irqLine(0);
    }
    return;

  case 0x4025:
    io.enableMotor = data.bit(0);
    bool reset     = data.bit(1);
    io.accessMode  = data.bit(2);
    io.mirroring   = data.bit(3);
    io.crcControl  = data.bit(5);
    irq.onTransfer = data.bit(7);
    return;

  }
}
