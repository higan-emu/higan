auto CPU::readIO(uint8 address) -> uint8 {
  uint8 data = 0xff;

  switch(address) {
  //ADREG04L
  case 0x60:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[0].bits(0,1);
    adc.end = 0;
    break;

  //ADREG04H
  case 0x61:
    data.bits(0,7) = adc.result[0].bits(2,9);
    adc.end = 0;
    intad.clear();
    break;

  //ADREG15L
  case 0x62:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[1].bits(0,1);
    adc.end = 0;
    break;

  //ADREG15H
  case 0x63:
    data.bits(0,7) = adc.result[1].bits(2,9);
    adc.end = 0;
    intad.clear();
    break;

  //ADREG2L
  case 0x64:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[2].bits(0,1);
    adc.end = 0;
    break;

  //ADREG2H
  case 0x65:
    data.bits(0,7) = adc.result[2].bits(2,9);
    adc.end = 0;
    intad.clear();
    break;

  //ADREG3L
  case 0x66:
    data.bits(0,5) = 0b111111;
    data.bits(6,7) = adc.result[3].bits(0,1);
    adc.end = 0;
    break;

  //ADREG3H
  case 0x67:
    data.bits(0,7) = adc.result[3].bits(2,9);
    adc.end = 0;
    intad.clear();
    break;

  //ADMOD
  case 0x6d:
    data.bits(0,1) = adc.channel;
    data.bit(2) = 0;  //always reads as zero
    data.bit(3) = adc.speed;
    data.bit(4) = adc.scan;
    data.bit(5) = adc.repeat;
    data.bit(6) = adc.busy;
    data.bit(7) = adc.end;
    break;

  //INTE0AD
  case 0x70:
    data.bits(0,2) = 0b000;
    data.bit(3) = int0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intad.request;
    break;

  //INTE45
  case 0x71:
    data.bits(0,2) = 0b000;
    data.bit(3) = int4.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = int5.request;
    break;

  //INTE67
  case 0x72:
    data.bits(0,2) = 0b000;
    data.bit(3) = int6.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = int7.request;
    break;

  //INTET01
  case 0x73:
    data.bits(0,2) = 0b000;
    data.bit(3) = intt0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intt1.request;
    break;

  //INTET23
  case 0x74:
    data.bits(0,2) = 0b000;
    data.bit(3) = intt2.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = intt3.request;
    break;

  //INTET45
  case 0x75:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttr4.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttr5.request;
    break;

  //INTET67
  case 0x76:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttr6.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttr7.request;
    break;

  //INTE50
  case 0x77:
    data.bits(0,2) = 0b000;
    data.bit(3) = intrx0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttx0.request;
    break;

  //INTE51
  case 0x78:
    data.bits(0,2) = 0b000;
    data.bit(3) = intrx1.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttx1.request;
    break;

  //INTETC01
  case 0x79:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttc0.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttc1.request;
    break;

  //INTETC23
  case 0x7a:
    data.bits(0,2) = 0b000;
    data.bit(3) = inttc2.request;
    data.bits(4,6) = 0b000;
    data.bit(7) = inttc3.request;
    break;

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

  //ADMOD
  case 0x6d: {
    uint1 busy = adc.busy;
    adc.channel = data.bits(0,1);
    uint1 start = data.bit(2);
    adc.speed = data.bit(3);
    adc.scan = data.bit(4);
    adc.repeat = data.bit(5);

    if(!busy && start) {
      adc.busy = 1;
      adc.counter = !adc.speed ? 160 : 320;
    }
  } return;

  //WDCR
  case 0x6f:
    if(data == 0x4e);  //clear
    if(data == 0xb1);  //disable
    return;

  //INTE0AD
  case 0x70:
    int0.priority = data.bits(0,2);
    if(!data.bit(3)) int0.clear();
    intad.priority = data.bits(4,6);
    if(!data.bit(7)) intad.clear();
    return;

  //INTE45
  case 0x71:
    int4.priority = data.bits(0,2);
    if(!data.bit(3)) int4.clear();
    int5.priority = data.bits(4,6);
    if(!data.bit(7)) int5.clear();
    return;

  //INTE67
  case 0x72:
    int6.priority = data.bits(0,2);
    if(!data.bit(3)) int6.clear();
    int7.priority = data.bits(4,6);
    if(!data.bit(7)) int7.clear();
    return;

  //INTET01
  case 0x73:
    intt0.priority = data.bits(0,2);
    if(!data.bit(3)) intt0.clear();
    intt1.priority = data.bits(4,6);
    if(!data.bit(7)) intt1.clear();
    return;

  //INTET23
  case 0x74:
    intt2.priority = data.bits(0,2);
    if(!data.bit(3)) intt2.clear();
    intt3.priority = data.bits(4,6);
    if(!data.bit(7)) intt3.clear();
    return;

  //INTET45
  case 0x75:
    inttr4.priority = data.bits(0,2);
    if(!data.bit(3)) inttr4.clear();
    inttr5.priority = data.bits(4,6);
    if(!data.bit(7)) inttr5.clear();
    return;

  //INTET67
  case 0x76:
    inttr6.priority = data.bits(0,2);
    if(!data.bit(3)) inttr6.clear();
    inttr7.priority = data.bits(4,6);
    if(!data.bit(7)) inttr7.clear();
    return;

  //INTE50
  case 0x77:
    intrx0.priority = data.bits(0,2);
    if(!data.bit(3)) intrx0.clear();
    inttx0.priority = data.bits(4,6);
    if(!data.bit(7)) inttx0.clear();
    return;

  //INTE51
  case 0x78:
    intrx1.priority = data.bits(0,2);
    if(!data.bit(3)) intrx1.clear();
    inttx1.priority = data.bits(4,6);
    if(!data.bit(7)) inttx1.clear();
    return;

  //INTETC01
  case 0x79:
    inttc0.priority = data.bits(0,2);
    if(!data.bit(3)) inttc0.clear();
    inttc1.priority = data.bits(4,6);
    if(!data.bit(7)) inttc1.clear();
    return;

  //INTETC23
  case 0x7a:
    inttc2.priority = data.bits(0,2);
    if(!data.bit(3)) inttc2.clear();
    inttc3.priority = data.bits(4,6);
    if(!data.bit(7)) inttc3.clear();
    return;

  //???
  case 0xb2:
    return;

  //case 0xb3:
    //BIOS sets IFF=5, 0xb3.d1=1 when booting from (0xffff00) and then halts execution
    //the only interrupt with a non-zero level is INT0=5
    //the BIOS still doesn't run properly even with this, however ...
    //int0.line = data.bit(1);
    //return;

  case 0xb9:
    if(data == 0x55) {
      apu.enable();
    }
    if(data == 0xaa) {
      apu.disable();
    }
    return;

  case 0xba:
    apu.nmi.line = 1;
    return;

  case 0xbc:
    io.apuPort = data;
    return;
  }

//use to detect unimplemented internal registers
//print("CPU::writeIO(", hex(address, 2L), " = ", hex(data, 2L), ")\n");
}
