auto CPU::keypadRead() -> uint4 {
  uint4 data = 0;

  if(r.ypadEnable) {
    if(system.video.orientation->value() == "Horizontal") {
      data.bit(0) = system.controls.y1->value;
      data.bit(1) = system.controls.y2->value;
      data.bit(2) = system.controls.y3->value;
      data.bit(3) = system.controls.y4->value;
    } else {
      data.bit(0) = system.controls.x4->value;
      data.bit(1) = system.controls.x1->value;
      data.bit(2) = system.controls.x2->value;
      data.bit(3) = system.controls.x3->value;
    }
  }

  if(r.xpadEnable) {
    if(system.video.orientation->value() == "Horizontal") {
      data.bit(0) = system.controls.x1->value;
      data.bit(1) = system.controls.x2->value;
      data.bit(2) = system.controls.x3->value;
      data.bit(3) = system.controls.x4->value;
    } else {
      data.bit(0) = system.controls.y4->value;
      data.bit(1) = system.controls.y1->value;
      data.bit(2) = system.controls.y2->value;
      data.bit(3) = system.controls.y3->value;
    }
  }

  if(r.buttonEnable) {
    data.bit(1) = system.controls.start->value;
    data.bit(2) = system.controls.a->value;
    data.bit(3) = system.controls.b->value;
  }

  return data;
}

auto CPU::portRead(uint16 address) -> uint8 {
  uint8 data;

  //DMA_SRC
  if(address == 0x0040) return r.dmaSource.byte(0);
  if(address == 0x0041) return r.dmaSource.byte(1);
  if(address == 0x0042) return r.dmaSource.byte(2);

  //DMA_DST
  if(address == 0x0044) return r.dmaTarget.byte(0);
  if(address == 0x0045) return r.dmaTarget.byte(1);

  //DMA_LEN
  if(address == 0x0046) return r.dmaLength.byte(0);
  if(address == 0x0047) return r.dmaLength.byte(1);

  //DMA_CTRL
  if(address == 0x0048) {
    data.bit(0) = r.dmaMode;
    data.bit(7) = r.dmaEnable;
    return data;
  }

  //WSC_SYSTEM
  if(address == 0x0062) {
    data.bit(7) = Model::SwanCrystal() || Model::MamaMitte();
    return data;
  }

  //HW_FLAGS
  if(address == 0x00a0) {
    data.bit(0) = r.cartridgeEnable;
    data.bit(1) = Model::WonderSwanColor() || Model::SwanCrystal() || Model::MamaMitte();
    data.bit(2) = 1;  //0 = 8-bit bus width; 1 = 16-bit bus width
    data.bit(7) = 1;  //1 = built-in self-test passed
    return data;
  }

  //INT_BASE
  if(address == 0x00b0) {
    data  = r.interruptBase;
    data |= Model::WonderSwan() || Model::PocketChallengeV2() ? 3 : 0;
    return data;
  }

  //SER_DATA
  if(address == 0x00b1) return r.serialData;

  //INT_ENABLE
  if(address == 0x00b2) return r.interruptEnable;

  //SER_STATUS
  if(address == 0x00b3) {
    data.bit(2) = 1;  //hack: always report send buffer as empty
    data.bit(6) = r.serialBaudRate;
    data.bit(7) = r.serialEnable;
    return data;
  }

  //INT_STATUS
  if(address == 0x00b4) return r.interruptStatus;

  //KEYPAD
  if(address == 0x00b5) {
    data.bit(0,3) = keypadRead();
    data.bit(4)   = r.ypadEnable;
    data.bit(5)   = r.xpadEnable;
    data.bit(6)   = r.buttonEnable;
    return data;
  }

  return data;
}

auto CPU::portWrite(uint16 address, uint8 data) -> void {
  //DMA_SRC
  if(address == 0x0040) r.dmaSource.byte(0) = data & ~1;
  if(address == 0x0041) r.dmaSource.byte(1) = data;
  if(address == 0x0042) r.dmaSource.byte(2) = data;

  //DMA_DST
  if(address == 0x0044) r.dmaTarget.byte(0) = data & ~1;
  if(address == 0x0045) r.dmaTarget.byte(1) = data;

  //DMA_LEN
  if(address == 0x0046) r.dmaLength.byte(0) = data & ~1;
  if(address == 0x0047) r.dmaLength.byte(1) = data;

  //DMA_CTRL
  if(address == 0x0048) {
    r.dmaMode   = data.bit(6);
    r.dmaEnable = data.bit(7);
    if(r.dmaEnable) dmaTransfer();
  }

  //WSC_SYSTEM
  if(address == 0x0062) {
    //todo: d0 = 1 powers off system
  }

  //HW_FLAGS
  if(address == 0x00a0) {
    //todo: d2 (bus width) bit is writable; but ... it will do very bad things
    r.cartridgeEnable |= data.bit(0);  //bit can never be unset (boot ROM lockout)
  }

  //INT_BASE
  if(address == 0x00b0) {
    r.interruptBase = Model::WonderSwan() || Model::PocketChallengeV2() ? data & ~7 : data & ~1;
  }

  //SER_DATA
  if(address == 0x00b1) r.serialData = data;

  //INT_ENABLE
  if(address == 0x00b2) {
    r.interruptEnable = data;
    r.interruptStatus &= ~r.interruptEnable;
  }

  //SER_STATUS
  if(address == 0x00b3) {
    r.serialBaudRate = data.bit(6);
    r.serialEnable   = data.bit(7);
  }

  //KEYPAD
  if(address == 0x00b5) {
    r.ypadEnable   = data.bit(4);
    r.xpadEnable   = data.bit(5);
    r.buttonEnable = data.bit(6);
  }

  //INT_ACK
  if(address == 0x00b6) {
    //acknowledge only edge-sensitive interrupts
    r.interruptStatus &= ~(data & 0b11110010);
  }
}
