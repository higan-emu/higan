auto CPU::wramAddress(uint13 address) const -> uint16 {
  if(address < 0x1000) return address;
  auto bank = status.wramBank + (status.wramBank == 0);
  return bank << 12 | (uint12)address;
}

auto CPU::input(uint4 data) -> void {
  status.joyp = data;
}

auto CPU::joypPoll() -> void {
  if(!Model::SuperGameBoy()) {
    system.controls.poll();

    uint4 dpad;
    dpad.field(0) = !system.controls.rightLatch;
    dpad.field(1) = !system.controls.leftLatch;
    dpad.field(2) = !system.controls.upLatch;
    dpad.field(3) = !system.controls.downLatch;

    uint4 button;
    button.field(0) = !system.controls.a->value;
    button.field(1) = !system.controls.b->value;
    button.field(2) = !system.controls.select->value;
    button.field(3) = !system.controls.start->value;

    status.joyp = 0xf;
    if(status.p14 == 0) status.joyp &= dpad;
    if(status.p15 == 0) status.joyp &= button;
  }

  if(status.joyp != 0xf) raise(Interrupt::Joypad);
}

auto CPU::readIO(uint cycle, uint16 address, uint8 data) -> uint8 {
  if(address <= 0xbfff) return data;
  if(address >= 0xc000 && address <= 0xfdff && cycle == 2) return wram[wramAddress(address)];
  if(address >= 0xff80 && address <= 0xfffe && cycle == 2) return hram[(uint7)address];

  if(address == 0xff00 && cycle == 2) {  //JOYP
    joypPoll();
    data.field(0) = status.joyp.field(0);
    data.field(1) = status.joyp.field(1);
    data.field(2) = status.joyp.field(2);
    data.field(3) = status.joyp.field(3);
    data.field(4) = status.p14;
    data.field(5) = status.p15;
    return data;
  }

  if(address == 0xff01 && cycle == 2) {  //SB
    //unemulated
    return 0x00;
  }

  if(address == 0xff02 && cycle == 2) {  //SC
    data.field(0) = status.serialClock;
    data.field(1) = status.serialSpeed | !Model::GameBoyColor();
    data.field(7) = status.serialTransfer;
    return data;
  }

  if(address == 0xff04 && cycle == 2) {  //DIV
    return status.div.range(8,15);
  }

  if(address == 0xff05 && cycle == 2) {  //TIMA
    return status.tima;
  }

  if(address == 0xff06 && cycle == 2) {  //TMA
    return status.tma;
  }

  if(address == 0xff07 && cycle == 2) {  //TAC
    data.field(0) = status.timerClock.field(0);
    data.field(1) = status.timerClock.field(1);
    data.field(2) = status.timerEnable;
    return data;
  }

  if(address == 0xff0f && cycle == 2) {  //IF
    data.field(0) = status.interruptFlag.field(0);
    data.field(1) = status.interruptFlag.field(1);
    data.field(2) = status.interruptFlag.field(2);
    data.field(3) = status.interruptFlag.field(3);
    data.field(4) = status.interruptFlag.field(4);
    return data;
  }

  if(Model::GameBoyColor())
  if(address == 0xff4d && cycle == 2) {  //KEY1
    data.field(0) = status.speedSwitch;
    data.field(7) = status.speedDouble;
    return data;
  }

  if(Model::GameBoyColor())
  if(address == 0xff55 && cycle == 2) {  //HDMA5
    data.range(0,6) = status.dmaLength / 16 - 1 & 0x7f;
    data.field(7)   = status.dmaCompleted;
    return data;
  }

  if(Model::GameBoyColor())
  if(address == 0xff56 && cycle == 2) {  //RP
    //unemulated
    return 0x02;
  }

  if(Model::GameBoyColor())
  if(address == 0xff6c && cycle == 2) {  //???
    data.field(0) = status.ff6c;
    return data;
  }

  if(Model::GameBoyColor())
  if(address == 0xff70 && cycle == 2) {  //SVBK
    return status.wramBank;
  }

  if(Model::GameBoyColor())
  if(address == 0xff72 && cycle == 2) {  //???
    return status.ff72;
  }

  if(Model::GameBoyColor())
  if(address == 0xff73 && cycle == 2) {  //???
    return status.ff73;
  }

  if(Model::GameBoyColor())
  if(address == 0xff74 && cycle == 2) {  //???
    return status.ff74;
  }

  if(Model::GameBoyColor())
  if(address == 0xff75 && cycle == 2) {  //???
    data.range(4,6) = status.ff75;
    return data;
  }

  if(Model::GameBoyColor())
  if(address == 0xff76 && cycle == 2) {  //???
    return 0xff;
  }

  if(Model::GameBoyColor())
  if(address == 0xff77 && cycle == 2) {  //???
    return 0xff;
  }

  if(address == 0xffff && cycle == 2) {  //IE
    return status.interruptEnable;
  }

  return data;
}

auto CPU::writeIO(uint cycle, uint16 address, uint8 data) -> void {
  if(address <= 0xbfff) return;
  if(address >= 0xc000 && address <= 0xfdff && cycle == 2) { wram[wramAddress(address)] = data; return; }
  if(address >= 0xff80 && address <= 0xfffe && cycle == 2) { hram[(uint7)address] = data; return; }

  if(address == 0xff00 && cycle == 2) {  //JOYP
    status.p14 = data.field(4);
    status.p15 = data.field(5);
    if(Model::SuperGameBoy()) superGameBoy->joypWrite(status.p14, status.p15);
    return;
  }

  if(address == 0xff01 && cycle == 2) {  //SB
    status.serialData = data;
    return;
  }

  if(address == 0xff02 && cycle == 2) {  //SC
    status.serialClock    = data.field(0);
    status.serialSpeed    = data.field(1) & Model::GameBoyColor();
    status.serialTransfer = data.field(7);
    if(status.serialTransfer) status.serialBits = 8;
    return;
  }

  if(address == 0xff04 && cycle == 2) {  //DIV
    status.div = 0;
    return;
  }

  if(address == 0xff05 && cycle == 2) {  //TIMA
    status.tima = data;
    return;
  }

  if(address == 0xff06 && cycle == 2) {  //TMA
    status.tma = data;
    return;
  }

  if(address == 0xff07 && cycle == 2) {  //TAC
    status.timerClock  = data.range(0,1);
    status.timerEnable = data.field(2);
    return;
  }

  if(address == 0xff0f && cycle == 2) {  //IF
    status.interruptFlag = data.range(0,4);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff4d && cycle == 2) {  //KEY1
    status.speedSwitch = data.field(0);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff51 && cycle == 2) {  //HDMA1
    status.dmaSource.range(8,15) = data.range(0,7);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff52 && cycle == 2) {  //HDMA2
    status.dmaSource.range(4,7) = data.range(4,7);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff53 && cycle == 2) {  //HDMA3
    status.dmaTarget.range(8,15) = data.range(0,7);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff54 && cycle == 2) {  //HDMA4
    status.dmaTarget.range(4,7) = data.range(4,7);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff55 && cycle == 2) {  //HDMA5
    status.dmaLength    = (data.range(0,6) + 1) * 16;
    status.dmaMode      = data.field(7);
    status.dmaCompleted = !status.dmaMode;

    if(status.dmaMode == 0) {
      do {
        for(uint loop : range(16)) {
          writeDMA(status.dmaTarget++, readDMA(status.dmaSource++, 0xff));
        }
        step(8 << status.speedDouble);
        status.dmaLength -= 16;
      } while(status.dmaLength);
    }
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff56 && cycle == 2) {  //RP
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff6c && cycle == 2) {  //???
    status.ff6c = data.field(0);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff72 && cycle == 2) {  //???
    status.ff72 = data;
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff73 && cycle == 2) {  //???
    status.ff73 = data;
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff74 && cycle == 2) {  //???
    status.ff74 = data;
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff75 && cycle == 2) {  //???
    status.ff75 = data.range(4,6);
    return;
  }

  if(Model::GameBoyColor())
  if(address == 0xff70 && cycle == 2) {  //SVBK
    status.wramBank = data.range(0,3);
    return;
  }

  if(address == 0xffff && cycle == 2) {  //IE
    status.interruptEnable = data;
    return;
  }
}
