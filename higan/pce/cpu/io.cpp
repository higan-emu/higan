auto CPU::read(uint8 bank, uint13 address) -> uint8 {
  uint8 data = io.mdr;

  //$00-7f  HuCard
  if(!bank.bit(7)) {
    return cartridge.read(bank << 13 | address);
  }

  //$f7  BRAM
  if(bank == 0xf7) {
    return bram[address.bit(0,10)];
  }

  //$f8-fb  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    if(Model::PCEngine()) return ram[address];
    if(Model::SuperGrafx()) return ram[bank.bit(0,1) << 13 | address];
  }

  //$ff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC or VPC
    if((address & 0x1c00) == 0x0000) {
      step(1);  //penalty cycle
      if(Model::PCEngine()) return vdp.vdc0.read(address);
      if(Model::SuperGrafx()) return vdp.vpc.read(address);
    }

    //$0400-07ff  VCE
    if((address & 0x1c00) == 0x0400) {
      step(1);  //penalty cycle
      return vdp.vce.read(address);
    }

    //$0800-0bff  PSG
    if((address & 0x1c00) == 0x0800) {
      return data;
    }

    //$0c00-0fff  Timer
    if((address & 0x1c00) == 0x0c00) {
      data.bit(0,6) = timer.value;
      return data;
    }

    //$1000-13ff  I/O
    if((address & 0x1c00) == 0x1000) {
      //note 1: Turbografx-16 games check this bit for region protection.
      //yet PC Engine games do not. since we cannot tell the games apart,
      //it's more compatible to always identify as a Turbografx-16 system.
      //note 2: we state that the CD-ROM drive is present.
      //this is so games can use its backup RAM for save data.
      data.bit(0,3) = controllerPort.read();
      data.bit(4)   = 1;
      data.bit(5)   = 1;
      data.bit(6)   = 0;  //device (0 = Turbografx-16; 1 = PC Engine)
      data.bit(7)   = 0;  //add-on (0 = CD-ROM; 1 = nothing)
      return data;
    }

    //$1400-17ff  IRQ
    if((address & 0x1c00) == 0x1400) {
      if(address.bit(0,1) == 0) {
        return data;
      }

      if(address.bit(0,1) == 1) {
        return data;
      }

      if(address.bit(0,1) == 2) {
        data.bit(0) = irq.disableExternal;
        data.bit(1) = irq.disableVDC;
        data.bit(2) = irq.disableTimer;
        return data;
      }

      if(address.bit(0,1) == 3) {
        data.bit(0) = 0;  //pending external IRQ
        data.bit(1) = vdp.vdc0.irqLine() | vdp.vdc1.irqLine();
        data.bit(2) = timer.irqLine();
        return data;
      }
    }

    //$1800-1bff  CD-ROM
    if((address & 0x1c00) == 0x1800) {
      return 0xff;
    }

    //$1c00-1fff  unmapped
    if((address & 0x1c00) == 0x1c00) {
      return 0xff;
    }
  }

  return 0xff;
}

auto CPU::write(uint8 bank, uint13 address, uint8 data) -> void {
  //$00-7f  HuCard
  if(!bank.bit(7)) {
    return cartridge.write(bank << 13 | address, data);
  }

  //$f7  BRAM
  if(bank == 0xf7) {
    bram[address.bit(0,10)] = data;
    return;
  }

  //$f8-fb  RAM
  if(bank >= 0xf8 && bank <= 0xfb) {
    if(Model::PCEngine()) ram[address] = data;
    if(Model::SuperGrafx()) ram[bank.bit(0,1) << 13 | address] = data;
    return;
  }

  //$1fe000-1fffff  Hardware
  if(bank == 0xff) {
    //$0000-03ff  VDC or VPC
    if((address & 0x1c00) == 0x0000) {
      step(1);  //penalty cycle
      if(Model::PCEngine()) return vdp.vdc0.write(address, data);
      if(Model::SuperGrafx()) return vdp.vpc.write(address, data);
    }

    //$0400-07ff  VCE
    if((address & 0x1c00) == 0x0400) {
      step(1);  //penalty cycle
      return vdp.vce.write(address, data);
    }

    //$0800-0bff  PSG
    if((address & 0x1c00) == 0x0800) {
      io.mdr = data;
      return psg.write(address, data);
    }

    //$0c00-0fff  Timer
    if((address & 0x1c00) == 0x0c00) {
      io.mdr = data;
      if(!address.bit(0)) {
        timer.reload = data.bit(0,6);
      } else {
        if(!timer.enable && data.bit(0)) {
          timer.value = timer.reload;
          timer.counter = 1024 * 3;
        }
        timer.enable = data.bit(0);
      }
      return;
    }

    //$1000-13ff  I/O
    if((address & 0x1c00) == 0x1000) {
      io.mdr = data;
      controllerPort.write(data.bit(0,1));
      return;
    }

    //$1400-17ff  IRQ
    if((address & 0x1c00) == 0x1400) {
      io.mdr = data;
      if(address.bit(0,1) == 2) {
        irq.disableExternal = data.bit(0);
        irq.disableVDC = data.bit(1);
        irq.disableTimer = data.bit(2);
        return;
      }

      if(address.bit(0,1) == 3) {
        timer.line = 0;
        return;
      }
    }

    //$1800-1bff  CD-ROM
    if((address & 0x1c00) == 0x1800) {
      return;
    }

    //$1c00-1fff  unmapped
    if((address & 0x1c00) == 0x1c00) {
      return;
    }
  }
}

//ST0, ST1, ST2
auto CPU::store(uint2 address, uint8 data) -> void {
  step(1);  //penalty cycle
  if(address) address++;  //0,1,2 => 0,2,3
  if(Model::PCEngine()) vdp.vdc0.write(address, data);
  if(Model::SuperGrafx()) vdp.vpc.store(address, data);
}
