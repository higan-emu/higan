auto MCD::external_read(uint1 upper, uint1 lower, uint22 address, uint16 data) -> uint16 {
  address.bits(18,20) = 0;  //mirrors

  if(address >= 0x000000 && address <= 0x01ffff) {
    return bios[address >> 1];
  }

  if(address >= 0x020000 && address <= 0x03ffff) {
    address = io.pramBank << 17 | (uint17)address;
    return pram[address >> 1];
  }

  if(address >= 0x200000 && address <= 0x23ffff && io.wramMode == 0) {
    if(io.wramSwitch == 1) return data;
    return wram[address >> 1];
  }

  if(address >= 0x200000 && address <= 0x23ffff && io.wramMode == 1) {
    address = !io.wramSelect << 17 | (uint17)address;
    return wram[address >> 1];
  }

  return data;
}

auto MCD::external_write(uint1 upper, uint1 lower, uint22 address, uint16 data) -> void {
  address.bits(18,20) = 0;  //mirrors

  if(address >= 0x020000 && address <= 0x03ffff) {
    address = io.pramBank << 17 | (uint17)address;
    if(upper) pram[address >> 1].byte(1) = data.byte(1);
    if(lower) pram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x200000 && address <= 0x23ffff && io.wramMode == 0) {
    if(io.wramSwitch == 1) return;
    if(upper) wram[address >> 1].byte(1) = data.byte(1);
    if(lower) wram[address >> 1].byte(0) = data.byte(0);
    return;
  }

  if(address >= 0x200000 && address <= 0x23ffff && io.wramMode == 1) {
    address = !io.wramSelect << 17 | (uint17)address;
    if(upper) wram[address >> 1].byte(1) = data.byte(1);
    if(lower) wram[address >> 1].byte(0) = data.byte(0);
    return;
  }
}

//

auto MCD::external_readIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  if(upper) data.byte(1) = external_readIO(address | 0, data.byte(1));
  if(lower) data.byte(0) = external_readIO(address | 1, data.byte(0));
  return data;
}

auto MCD::external_writeIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  if(upper) external_writeIO(address | 0, data.byte(1));
  if(lower) external_writeIO(address | 1, data.byte(0));
}

//

auto MCD::external_readIO(uint24 address, uint8 data) -> uint8 {
  address.bits(6,7) = 0;  //a12040-a120ff mirrors a12000-a1203f

  switch(address) {
  case 0xa12000:
    data.bit(0) = irq.external.pending;
    break;

  case 0xa12001:
    data.bit(0) = io.run;
    data.bit(1) = io.request;
    break;

  case 0xa12002:
    data = io.pramProtect;
    break;

  case 0xa12003:
    data.bit(0) =!io.wramMode ? !io.wramSwitch : +io.wramSelect;
    data.bit(1) = io.wramSwitch;
    data.bit(2) = io.wramMode;
    data.bits(6,7) = io.pramBank;
    break;

  case 0xa12006:
    data = bios.read(0x72);
    break;

  case 0xa12007:
    data = bios.read(0x73);
    break;

  case 0xa1200c:
    data.bits(0,3) = cdc.stopwatch.bits(8,11);
    break;

  case 0xa1200d:
    data.bits(0,7) = cdc.stopwatch.bits(0, 7);
    break;

  case 0xa1200e:
    data = communication.cfm;
    break;

  case 0xa1200f:
    data = communication.cfs;
    break;

  case 0xa12010: data = communication.command[ 0]; break;
  case 0xa12011: data = communication.command[ 1]; break;
  case 0xa12012: data = communication.command[ 2]; break;
  case 0xa12013: data = communication.command[ 3]; break;
  case 0xa12014: data = communication.command[ 4]; break;
  case 0xa12015: data = communication.command[ 5]; break;
  case 0xa12016: data = communication.command[ 6]; break;
  case 0xa12017: data = communication.command[ 7]; break;
  case 0xa12018: data = communication.command[ 8]; break;
  case 0xa12019: data = communication.command[ 9]; break;
  case 0xa1201a: data = communication.command[10]; break;
  case 0xa1201b: data = communication.command[11]; break;
  case 0xa1201c: data = communication.command[12]; break;
  case 0xa1201d: data = communication.command[13]; break;
  case 0xa1201e: data = communication.command[14]; break;
  case 0xa1201f: data = communication.command[15]; break;

  case 0xa12020: data = communication.status[ 0]; break;
  case 0xa12021: data = communication.status[ 1]; break;
  case 0xa12022: data = communication.status[ 2]; break;
  case 0xa12023: data = communication.status[ 3]; break;
  case 0xa12024: data = communication.status[ 4]; break;
  case 0xa12025: data = communication.status[ 5]; break;
  case 0xa12026: data = communication.status[ 6]; break;
  case 0xa12027: data = communication.status[ 7]; break;
  case 0xa12028: data = communication.status[ 8]; break;
  case 0xa12029: data = communication.status[ 9]; break;
  case 0xa1202a: data = communication.status[10]; break;
  case 0xa1202b: data = communication.status[11]; break;
  case 0xa1202c: data = communication.status[12]; break;
  case 0xa1202d: data = communication.status[13]; break;
  case 0xa1202e: data = communication.status[14]; break;
  case 0xa1202f: data = communication.status[15]; break;
  }

  return data;
}

auto MCD::external_writeIO(uint24 address, uint8 data) -> void {
  address.bits(6,7) = 0;  //a12040-a120ff mirrors a12000-a1203f

  switch(address) {
  case 0xa12000:
    if(data.bit(0)) irq.external.raise();
    break;

  case 0xa12001:
    if(io.run && !data.bit(0)) power(true);
    io.run     = data.bit(0);
    io.request = data.bit(1);
    io.halt    = !io.run || io.request;
    break;

  case 0xa12002:
    io.pramProtect = data;
    break;

  case 0xa12003:
    if(data.bit(1)) io.wramSwitch = 1;
    io.pramBank = data.bits(6,7);
    break;

  case 0xa12006: {
    uint16 vector = bios.read(0x72 >> 1);
    vector.byte(1) = data;
    bios.program(0x72 >> 1, vector);
  } break;

  case 0xa12007: {
    uint16 vector = bios.read(0x72 >> 1);
    vector.byte(0) = data;
    bios.program(0x72 >> 1, vector);
  } break;

  case 0xa1200e:
    communication.cfm = data;
    break;

  case 0xa12010: communication.command[ 0] = data; break;
  case 0xa12011: communication.command[ 1] = data; break;
  case 0xa12012: communication.command[ 2] = data; break;
  case 0xa12013: communication.command[ 3] = data; break;
  case 0xa12014: communication.command[ 4] = data; break;
  case 0xa12015: communication.command[ 5] = data; break;
  case 0xa12016: communication.command[ 6] = data; break;
  case 0xa12017: communication.command[ 7] = data; break;
  case 0xa12018: communication.command[ 8] = data; break;
  case 0xa12019: communication.command[ 9] = data; break;
  case 0xa1201a: communication.command[10] = data; break;
  case 0xa1201b: communication.command[11] = data; break;
  case 0xa1201c: communication.command[12] = data; break;
  case 0xa1201d: communication.command[13] = data; break;
  case 0xa1201e: communication.command[14] = data; break;
  case 0xa1201f: communication.command[15] = data; break;
  }
}
