auto MCD::external_readIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  address.bits(6,7) = 0;  //a12040-a120ff mirrors a12000-a1203f

  if(address == 0xa12000) {
    data.bit ( 0)    = io.run;
    data.bit ( 1)    = io.request;
    data.bits( 2, 7) = Unmapped;
    data.bit ( 8)    = external.irq.pending;
    data.bits( 9,14) = Unmapped;
    //todo: hardware manual states this bit exists; software manual states that it does not
    data.bit (15)    = external.irq.enable;
  }

  if(address == 0xa12002) {
    data.bit (0)    =!io.wramMode ? !io.wramSwitch : +io.wramSelect;
    data.bit (1)    = io.wramSwitch;
    data.bit (2)    = io.wramMode;
    data.bits(3, 5) = Unmapped;
    data.bits(6, 7) = io.pramBank;
    data.bits(8,15) = io.pramProtect;
  }

  if(address == 0xa12004) {
    print("* read a12004\n");
  }

  if(address == 0xa12006) {
    data = bios.read(0x72 >> 1);
  }

  if(address == 0xa12008) {
    print("* read a12008\n");
  }

  if(address == 0xa1200a) {
    print("* read a1200a\n");
  }

  if(address == 0xa1200c) {
    data.bits( 0,11) = cdc.stopwatch;
    data.bits(12,15) = Unmapped;
  }

  if(address == 0xa1200e) {
    data.byte(0) = communication.cfs;
    data.byte(1) = communication.cfm;
  }

  if(address >= 0xa12010 && address <= 0xa1201f) {
    uint3 index = address - 0xa12010 >> 1;
    data = communication.command[index];
  }

  if(address >= 0xa12020 && address <= 0xa1202f) {
    uint3 index = address - 0xa12020 >> 1;
    data = communication.status[index];
  }

  return data;
}

auto MCD::external_writeIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  address.bits(6,7) = 0;  //a12040-a120ff mirrors a12000-a1203f

  if(address == 0xa12000) {
    if(lower) {
      if(io.run && !data.bit(0)) power(true);
      io.run     = data.bit(0);
      io.request = data.bit(1);
      io.halt    = !io.run || io.request;
    }
    if(upper) {
      if(data.bit(8)) external.irq.raise();
    }
  }

  if(address == 0xa12002) {
    if(lower) {
      if(data.bit(1)) io.wramSwitch = 1;
      io.pramBank = data.bits(6,7);
    }
    if(upper) {
      io.pramProtect = data.bits(8,15);
    }
  }

  if(address == 0xa12004) {
    //read-only
  }

  if(address == 0xa12006) {
    bios.program(0x72 >> 1, data);
  }

  if(address == 0xa12008) {
    //read-only
  }

  if(address == 0xa1200a) {
    //reserved
    print("* write a1200a\n");
  }

  if(address == 0xa1200c) {
    //read-only
  }

  if(address == 0xa1200e) {
    if(upper) {  //unconfirmed
      communication.cfm = data.byte(1);
    }
  }

  if(address >= 0xa12010 && address <= 0xa1201f) {
    uint3 index = address - 0xa12010 >> 1;
    if(lower) communication.command[index].byte(0) = data.byte(0);
    if(upper) communication.command[index].byte(1) = data.byte(1);
  }

  if(address >= 0xa12020 && address <= 0xa1202f) {
    //read-only
  }
}
