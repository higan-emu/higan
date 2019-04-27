auto MCD::readIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  if(upper) data.byte(1) = readIO(address | 0);
  if(lower) data.byte(0) = readIO(address | 1);
  return data;
}

auto MCD::writeIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  if(upper) writeIO(address | 0, data.byte(1));
  if(lower) writeIO(address | 1, data.byte(0));
}

//

auto MCD::readIO(uint24 address) -> uint8 {
  uint8 data;

  switch(0xff8000 | (uint9)address) {
  case 0xff8000:
    data.bit(0) = led.red;
    data.bit(1) = led.green;
    break;

  case 0xff8001:
    data.bit(0) = 1;  //peripheral ready
    data.bits(4,7) = 0b0000;  //version#
    break;

  case 0xff8002:
    data = io.pramProtect;
    break;

  case 0xff8003:
    data.bit(0) =!io.wramMode ? !io.wramSwitch : +io.wramSelect;
    data.bit(1) = io.wramSwitch;
    data.bit(2) = io.wramMode;
    break;

  case 0xff8004:
    data.bits(0,2) = cdc.destination;
    data.bit (6)   = cdc.dsr;
    data.bit (7)   = cdc.edt;
    break;

  case 0xff8005:
    data.bits(0,3) = cdc.address;
    break;

  case 0xff8007:
    data.bits(0,7) = cdc.read();
    break;

  case 0xff800c:
    data.bits(0,3) = cdc.stopwatch.bits(8,11);
    break;

  case 0xff800d:
    data.bits(0,7) = cdc.stopwatch.bits(0,7);
    break;

  case 0xff800e:
    data = communication.cfm;
    break;

  case 0xff800f:
    data = communication.cfs;
    break;

  case 0xff8010: data = communication.command[ 0]; break;
  case 0xff8011: data = communication.command[ 1]; break;
  case 0xff8012: data = communication.command[ 2]; break;
  case 0xff8013: data = communication.command[ 3]; break;
  case 0xff8014: data = communication.command[ 4]; break;
  case 0xff8015: data = communication.command[ 5]; break;
  case 0xff8016: data = communication.command[ 6]; break;
  case 0xff8017: data = communication.command[ 7]; break;
  case 0xff8018: data = communication.command[ 8]; break;
  case 0xff8019: data = communication.command[ 9]; break;
  case 0xff801a: data = communication.command[10]; break;
  case 0xff801b: data = communication.command[11]; break;
  case 0xff801c: data = communication.command[12]; break;
  case 0xff801d: data = communication.command[13]; break;
  case 0xff801e: data = communication.command[14]; break;
  case 0xff801f: data = communication.command[15]; break;

  case 0xff8020: data = communication.status[ 0]; break;
  case 0xff8021: data = communication.status[ 1]; break;
  case 0xff8022: data = communication.status[ 2]; break;
  case 0xff8023: data = communication.status[ 3]; break;
  case 0xff8024: data = communication.status[ 4]; break;
  case 0xff8025: data = communication.status[ 5]; break;
  case 0xff8026: data = communication.status[ 6]; break;
  case 0xff8027: data = communication.status[ 7]; break;
  case 0xff8028: data = communication.status[ 8]; break;
  case 0xff8029: data = communication.status[ 9]; break;
  case 0xff802a: data = communication.status[10]; break;
  case 0xff802b: data = communication.status[11]; break;
  case 0xff802c: data = communication.status[12]; break;
  case 0xff802d: data = communication.status[13]; break;
  case 0xff802e: data = communication.status[14]; break;
  case 0xff802f: data = communication.status[15]; break;

  case 0xff8031:
    data = timer.counter;
    break;

  case 0xff8033:
    data.bit(1) = irq.graphics.enable;
    data.bit(2) = irq.external.enable;
    data.bit(3) = timer.irq.enable;
    data.bit(4) = cdd.irq.enable;
    data.bit(5) = cdc.irq.enable;
    data.bit(6) = irq.subcode.enable;
    break;

  case 0xff8034:
    data.bit(7) = 0;  //end of fade data transfer
    break;

  case 0xff8036:
    data.bit(0) = 1;  //0 = music, 1 = data
    break;

  case 0xff8037:
    data.bit(1) = 0;
    data.bit(2) = cdd.hock;
  //cdd.hock=0;
    break;

  case 0xff8038: data.bits(0,3) = cdd.status[0]; break;
  case 0xff8039: data.bits(0,3) = cdd.status[1]; break;
  case 0xff803a: data.bits(0,3) = cdd.status[2]; break;
  case 0xff803b: data.bits(0,3) = cdd.status[3]; break;
  case 0xff803c: data.bits(0,3) = cdd.status[4]; break;
  case 0xff803d: data.bits(0,3) = cdd.status[5]; break;
  case 0xff803e: data.bits(0,3) = cdd.status[6]; break;
  case 0xff803f: data.bits(0,3) = cdd.status[7]; break;
  case 0xff8040: data.bits(0,3) = cdd.status[8]; break;
  case 0xff8041: data.bits(0,3) = cdd.status[9]; break;

  case 0xff8042: data.bits(0,3) = cdd.command[0]; break;
  case 0xff8043: data.bits(0,3) = cdd.command[1]; break;
  case 0xff8044: data.bits(0,3) = cdd.command[2]; break;
  case 0xff8045: data.bits(0,3) = cdd.command[3]; break;
  case 0xff8046: data.bits(0,3) = cdd.command[4]; break;
  case 0xff8047: data.bits(0,3) = cdd.command[5]; break;
  case 0xff8048: data.bits(0,3) = cdd.command[6]; break;
  case 0xff8049: data.bits(0,3) = cdd.command[7]; break;
  case 0xff804a: data.bits(0,3) = cdd.command[8]; break;
  case 0xff804b: data.bits(0,3) = cdd.command[9]; break;
  }

//print("* r", hex(address, 6), " = ", hex(data, 2), "\n");
  return data;
}

auto MCD::writeIO(uint24 address, uint8 data) -> void {
  switch(0xff8000 | (uint9)address) {
  case 0xff8000:
    led.red   = data.bit(0);
    led.green = data.bit(1);
    break;

  case 0xff8003:
    io.wramSelect = data.bit(0);
    io.wramMode   = data.bit(2);
    io.wramSwitch = 0;
    break;

  case 0xff8004:
    cdc.destination = data.bits(0,2);
    break;

  case 0xff8005:
    cdc.address = data.bits(0,3);
    break;

  case 0xff8007:
    cdc.write(data);
    break;

  case 0xff800c:
    cdc.stopwatch = 0;
    break;

  case 0xff800d:
    cdc.stopwatch = 0;
    break;

  case 0xff800f:
    communication.cfs = data;
    break;

  case 0xff8020: communication.status[ 0] = data; break;
  case 0xff8021: communication.status[ 1] = data; break;
  case 0xff8022: communication.status[ 2] = data; break;
  case 0xff8023: communication.status[ 3] = data; break;
  case 0xff8024: communication.status[ 4] = data; break;
  case 0xff8025: communication.status[ 5] = data; break;
  case 0xff8026: communication.status[ 6] = data; break;
  case 0xff8027: communication.status[ 7] = data; break;
  case 0xff8028: communication.status[ 8] = data; break;
  case 0xff8029: communication.status[ 9] = data; break;
  case 0xff802a: communication.status[10] = data; break;
  case 0xff802b: communication.status[11] = data; break;
  case 0xff802c: communication.status[12] = data; break;
  case 0xff802d: communication.status[13] = data; break;
  case 0xff802e: communication.status[14] = data; break;
  case 0xff802f: communication.status[15] = data; break;

  case 0xff8031:
    timer.counter = data;
    break;

  case 0xff8033:
    irq.graphics.enable = data.bit(1);
    irq.external.enable = data.bit(2);
    timer.irq.enable    = data.bit(3);
    cdd.irq.enable      = data.bit(4);
    cdc.irq.enable      = data.bit(5);
    irq.subcode.enable  = data.bit(6);
  //if(!irq.graphics.enable) irq.graphics.lower();
    if(!irq.external.enable) irq.external.lower();
  //if(!timer.irq.enable   ) timer.irq.lower();
  //if(!cdd.irq.enable     ) cdd.irq.lower();
  //if(!cdc.irq.enable     ) cdc.irq.lower();
  //if(!irq.subcode.enable ) irq.subcode.lower();
    break;

  case 0xff8034:
    cdd.fader.volume.bits(4,10) = data.bits(0,6);
    break;

  case 0xff8035:
    cdd.fader.spindleSpeed = data.bit(1);
    cdd.fader.deemphasis = data.bits(2,3);
    cdd.fader.volume.bits(0,3) = data.bits(4,7);
    break;

  case 0xff8037:
  //if(!cdd.hock && data.bit(2)) cdd.irq.raise();
    cdd.hock = data.bit(2);
    cdd.counter = 0;
    cdd.irq.raise();
    break;

  case 0xff8038: cdd.status[0] = data.bits(0,3); break;
  case 0xff8039: cdd.status[1] = data.bits(0,3); break;
  case 0xff803a: cdd.status[2] = data.bits(0,3); break;
  case 0xff803b: cdd.status[3] = data.bits(0,3); break;
  case 0xff803c: cdd.status[4] = data.bits(0,3); break;
  case 0xff803d: cdd.status[5] = data.bits(0,3); break;
  case 0xff803e: cdd.status[6] = data.bits(0,3); break;
  case 0xff803f: cdd.status[7] = data.bits(0,3); break;
  case 0xff8040: cdd.status[8] = data.bits(0,3); break;
  case 0xff8041: cdd.status[9] = data.bits(0,3); break;

  case 0xff8042: cdd.command[0] = data.bits(0,3); break;
  case 0xff8043: cdd.command[1] = data.bits(0,3); break;
  case 0xff8044: cdd.command[2] = data.bits(0,3); break;
  case 0xff8045: cdd.command[3] = data.bits(0,3); break;
  case 0xff8046: cdd.command[4] = data.bits(0,3); break;
  case 0xff8047: cdd.command[5] = data.bits(0,3); break;
  case 0xff8048: cdd.command[6] = data.bits(0,3); break;
  case 0xff8049: cdd.command[7] = data.bits(0,3); break;
  case 0xff804a: cdd.command[8] = data.bits(0,3); break;
  case 0xff804b: cdd.command[9] = data.bits(0,3); cdd.process(); break;
  }

//print("* w", hex(address, 6), " = ", hex(data, 2), "\n");
}
