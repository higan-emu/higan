auto CDPU::readIO(uint24 address) -> uint8 {
  uint8 data;

  switch(address) {
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
    data.bit(2) = io.wramMode;
    break;

  case 0xff800c:
    data.bits(0,3) = io.stopwatch.bits(8,11);
    break;

  case 0xff800d:
    data.bits(0,7) = io.stopwatch.bits(0,7);
    break;

  case 0xff800e:
    data = io.cfm;
    break;

  case 0xff800f:
    data = io.cfs;
    break;

  case 0xff8031:
    data = io.timer;
    break;

  case 0xff8033:
    data.bit(1) = irq.graphics.enable;
    data.bit(2) = irq.external.enable;
    data.bit(3) = irq.timer.enable;
    data.bit(4) = irq.cdd.enable;
    data.bit(5) = irq.cdc.enable;
    data.bit(6) = irq.subcode.enable;
    break;

  case 0xff8034:
    data.bit(7) = 1;  //end of fade data transfer
    break;

  case 0xff8036:
    data.bit(0) = 1;  //0 = music, 1 = data
    break;

  case 0xff8037:
    data.bit(1) = 0;
    data.bit(2) = 1;
    break;
  }

  if(address >= 0xff8010 && address <= 0xff801f) data = io.command [address - 0xff8010];
  if(address >= 0xff8020 && address <= 0xff802f) data = io.status  [address - 0xff8020];
  if(address >= 0xff8038 && address <= 0xff8041) data = io.receive [address - 0xff8038];
  if(address >= 0xff8042 && address <= 0xff804b) data = io.transfer[address - 0xff8042];

//print("* r", hex(address, 6), " = ", hex(data, 2), "\n");
  return data;
}

auto CDPU::writeIO(uint24 address, uint8 data) -> void {
  switch(address) {
  case 0xff8000:
    led.red   = data.bit(0);
    led.green = data.bit(1);
    break;

  case 0xff8003:
    io.wramMode = data.bit(2);
    break;

  case 0xff800c:
    io.stopwatch = 0;
    break;

  case 0xff800d:
    io.stopwatch = 0;
    break;

  case 0xff800f:
    io.cfs = data;
    break;

  case 0xff8031:
    io.timer = data;
    break;

  case 0xff8033:
    irq.graphics.enable = data.bit(1);
    irq.external.enable = data.bit(2);
    irq.timer.enable    = data.bit(3);
    irq.cdd.enable      = data.bit(4);
    irq.cdc.enable      = data.bit(5);
    irq.subcode.enable  = data.bit(6);
    if(!irq.graphics.enable) irq.graphics.pending = 0;
    if(!irq.external.enable) irq.external.pending = 0;
    if(!irq.timer.enable   ) irq.timer.pending    = 0;
    if(!irq.cdd.enable     ) irq.cdd.pending      = 0;
    if(!irq.cdc.enable     ) irq.cdc.pending      = 0;
    if(!irq.subcode.enable ) irq.subcode.pending  = 0;
    irq.synchronize();
    break;

  case 0xff8037:
    if(data.bit(2)) irq.cdd.pending = irq.cdd.enable;
    break;
  }

  if(address >= 0xff8020 && address <= 0xff802f) io.status  [address - 0xff8020] = data;
  if(address >= 0xff8038 && address <= 0xff8041) io.receive [address - 0xff8038] = data;
  if(address >= 0xff8042 && address <= 0xff804b) io.transfer[address - 0xff8042] = data;

//print("* w", hex(address, 6), " = ", hex(data, 2), "\n");
}
