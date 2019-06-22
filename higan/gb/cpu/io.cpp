auto CPU::wramAddress(uint13 address) const -> uint16 {
  if(address < 0x1000) return address;
  auto bank = status.wramBank + (status.wramBank == 0);
  return bank << 12 | (uint12)address;
}

auto CPU::joypPoll() -> void {
  system.controls.poll();

  uint4 dpad;
  dpad.bit(0) = system.controls.rightLatch;
  dpad.bit(1) = system.controls.leftLatch;
  dpad.bit(2) = system.controls.upLatch;
  dpad.bit(3) = system.controls.downLatch;

  uint4 button;
  button.bit(0) = system.controls.a->value;
  button.bit(1) = system.controls.b->value;
  button.bit(2) = system.controls.select->value;
  button.bit(3) = system.controls.start->value;

  status.joyp = 0x0f;
  if(status.p15 == 1 && status.p14 == 1 && Model::SuperGameBoy()) {
    status.joyp = superGameBoy->joypRead();
  }
  if(status.p15 == 0) status.joyp &= button ^ 0x0f;
  if(status.p14 == 0) status.joyp &= dpad   ^ 0x0f;
  if(status.joyp != 0x0f) raise(Interrupt::Joypad);
}

auto CPU::readIO(uint16 address) -> uint8 {
  uint8 data = 0xff;

  if(address >= 0xc000 && address <= 0xfdff) return wram[wramAddress(address)];
  if(address >= 0xff80 && address <= 0xfffe) return hram[(uint7)address];

  if(address == 0xff00) {  //JOYP
    joypPoll();
    data.bit(0) = status.joyp.bit(0);
    data.bit(1) = status.joyp.bit(1);
    data.bit(2) = status.joyp.bit(2);
    data.bit(3) = status.joyp.bit(3);
    data.bit(4) = status.p14;
    data.bit(5) = status.p15;
    return data;
  }

  if(address == 0xff01) {  //SB
    //unemulated
    return 0x00;
  }

  if(address == 0xff02) {  //SC
    data.bit(0) = status.serialClock;
    data.bit(1) = status.serialSpeed | !Model::GameBoyColor();
    data.bit(7) = status.serialTransfer;
    return data;
  }

  if(address == 0xff04) {  //DIV
    return status.div >> 8;
  }

  if(address == 0xff05) {  //TIMA
    return status.tima;
  }

  if(address == 0xff06) {  //TMA
    return status.tma;
  }

  if(address == 0xff07) {  //TAC
    data.bit(0) = status.timerClock.bit(0);
    data.bit(1) = status.timerClock.bit(1);
    data.bit(2) = status.timerEnable;
    return data;
  }

  if(address == 0xff0f) {  //IF
    data.bit(0) = status.interruptFlag.bit(0);
    data.bit(1) = status.interruptFlag.bit(1);
    data.bit(2) = status.interruptFlag.bit(2);
    data.bit(3) = status.interruptFlag.bit(3);
    data.bit(4) = status.interruptFlag.bit(4);
    return data;
  }

  if(address == 0xff4d) {  //KEY1
    data.bit(0) = status.speedSwitch;
    data.bit(7) = status.speedDouble;
    return data;
  }

  if(address == 0xff55) {  //HDMA5
    data.bits(0,6) = status.dmaLength / 16 - 1 & 0x7f;
    data.bit (7)   = status.dmaCompleted;
    return data;
  }

  if(address == 0xff56) {  //RP
    //unemulated
    return 0x02;
  }

  if(address == 0xff6c) {  //???
    data.bit(0) = status.ff6c;
    return data;
  }

  if(address == 0xff70) {  //SVBK
    return status.wramBank;
  }

  if(address == 0xff72) {  //???
    return status.ff72;
  }

  if(address == 0xff73) {  //???
    return status.ff73;
  }

  if(address == 0xff74) {  //???
    return status.ff74;
  }

  if(address == 0xff75) {  //???
    data.bits(4,6) = status.ff75;
    return data;
  }

  if(address == 0xff76) {  //???
    return 0xff;
  }

  if(address == 0xff77) {  //???
    return 0xff;
  }

  if(address == 0xffff) {  //IE
    data = status.interruptEnable;
  }

  return data;
}

auto CPU::writeIO(uint16 address, uint8 data) -> void {
  if(address >= 0xc000 && address <= 0xfdff) { wram[wramAddress(address)] = data; return; }
  if(address >= 0xff80 && address <= 0xfffe) { hram[(uint7)address] = data; return; }

  if(address == 0xff00) {  //JOYP
    status.p14 = data.bit(4);
    status.p15 = data.bit(5);
    if(Model::SuperGameBoy()) superGameBoy->joypWrite(status.p14, status.p15);
    return;
  }

  if(address == 0xff01) {  //SB
    status.serialData = data;
    return;
  }

  if(address == 0xff02) {  //SC
    status.serialClock    = data.bit(0);
    status.serialSpeed    = data.bit(1) & Model::GameBoyColor();
    status.serialTransfer = data.bit(7);
    if(status.serialTransfer) status.serialBits = 8;
    return;
  }

  if(address == 0xff04) {  //DIV
    status.div = 0;
    return;
  }

  if(address == 0xff05) {  //TIMA
    status.tima = data;
    return;
  }

  if(address == 0xff06) {  //TMA
    status.tma = data;
    return;
  }

  if(address == 0xff07) {  //TAC
    status.timerClock  = data.bits(0,1);
    status.timerEnable = data.bit (2);
    return;
  }

  if(address == 0xff0f) {  //IF
    status.interruptFlag = data.bits(0,4);
    return;
  }

  if(address == 0xff4d) {  //KEY1
    status.speedSwitch = data.bit(0);
    return;
  }

  if(address == 0xff51) {  //HDMA1
    status.dmaSource.bits(8,15) = data.bits(0,7);
    return;
  }

  if(address == 0xff52) {  //HDMA2
    status.dmaSource.bits(4,7) = data.bits(4,7);
    return;
  }

  if(address == 0xff53) {  //HDMA3
    status.dmaTarget.bits(8,15) = data.bits(0,7);
    return;
  }

  if(address == 0xff54) {  //HDMA4
    status.dmaTarget.bits(4,7) = data.bits(4,7);
    return;
  }

  if(address == 0xff55) {  //HDMA5
    status.dmaLength    = (data.bits(0,6) + 1) * 16;
    status.dmaMode      = data.bit(7);
    status.dmaCompleted = !status.dmaMode;

    if(status.dmaMode == 0) {
      do {
        for(uint loop : range(16)) {
          writeDMA(status.dmaTarget++, readDMA(status.dmaSource++));
        }
        step(8 << status.speedDouble);
        status.dmaLength -= 16;
      } while(status.dmaLength);
    }
    return;
  }

  if(address == 0xff56) {  //RP
    return;
  }

  if(address == 0xff6c) {  //???
    status.ff6c = data.bit(0);
    return;
  }

  if(address == 0xff72) {  //???
    status.ff72 = data;
    return;
  }

  if(address == 0xff73) {  //???
    status.ff73 = data;
    return;
  }

  if(address == 0xff74) {  //???
    status.ff74 = data;
    return;
  }

  if(address == 0xff75) {  //???
    status.ff75 = data.bits(4,6);
    return;
  }

  if(address == 0xff70) {  //SVBK
    status.wramBank = data.bits(0,3);
    return;
  }

  if(address == 0xffff) {  //IE
    status.interruptEnable = data;
    return;
  }
}
