auto PPU::vramAddress(uint13 address) const -> uint16 {
  return status.vramBank << 13 | address;
}

auto PPU::readIO(uint16 address) -> uint8 {
  uint8 data = 0xff;

  if(address >= 0x8000 && address <= 0x9fff) {
    return vram[vramAddress(address)];
  }

  if(address >= 0xfe00 && address <= 0xfe9f) {
    if(status.dmaActive && status.dmaClock >= 8) return 0xff;
    return oam[(uint8)address];
  }

  if(address == 0xff40) {  //LCDC
    return (status.displayEnable << 7)
         | (status.windowTilemapSelect << 6)
         | (status.windowDisplayEnable << 5)
         | (status.bgTiledataSelect << 4)
         | (status.bgTilemapSelect << 3)
         | (status.obSize << 2)
         | (status.obEnable << 1)
         | (status.bgEnable << 0);
  }

  if(address == 0xff41) {  //STAT
    data.bit(0) = status.mode.bit(0);
    data.bit(1) = status.mode.bit(1);
    data.bit(2) = status.ly == status.lyc;
    data.bit(3) = status.interruptHblank;
    data.bit(4) = status.interruptVblank;
    data.bit(5) = status.interruptOAM;
    data.bit(6) = status.interruptLYC;
    data.bit(7) = 1;
  }

  if(address == 0xff42) {  //SCY
    return status.scy;
  }

  if(address == 0xff43) {  //SCX
    return status.scx;
  }

  if(address == 0xff44) {  //LY
    return status.ly;
  }

  if(address == 0xff45) {  //LYC
    return status.lyc;
  }

  if(address == 0xff46) {  //DMA
    data = status.dmaBank;
  }

  if(address == 0xff47) {  //BGP
    return (bgp[3] << 6)
         | (bgp[2] << 4)
         | (bgp[1] << 2)
         | (bgp[0] << 0);
  }

  if(address == 0xff48) {  //OBP0
    return (obp[0][3] << 6)
         | (obp[0][2] << 4)
         | (obp[0][1] << 2)
         | (obp[0][0] << 0);
  }

  if(address == 0xff49) {  //OBP1
    return (obp[1][3] << 6)
         | (obp[1][2] << 4)
         | (obp[1][1] << 2)
         | (obp[1][0] << 0);
  }

  if(address == 0xff4a) {  //WY
    return status.wy;
  }

  if(address == 0xff4b) {  //WX
    return status.wx;
  }

  if(address == 0xff4f) {  //VBK
    return status.vramBank;
  }

  if(address == 0xff68) {  //BGPI
    return status.bgpiIncrement << 7 | status.bgpi;
  }

  if(address == 0xff69) {  //BGPD
    return bgpd[status.bgpi];
  }

  if(address == 0xff6a) {  //OBPI
    return status.obpiIncrement << 7 | status.obpi;
  }

  if(address == 0xff6b) {  //OBPD
    return obpd[status.obpi];
  }

  return data;
}

auto PPU::writeIO(uint16 address, uint8 data) -> void {
  if(address >= 0x8000 && address <= 0x9fff) {
    vram[vramAddress(address)] = data;
    return;
  }

  if(address >= 0xfe00 && address <= 0xfe9f) {
    if(status.dmaActive && status.dmaClock >= 8) return;
    oam[(uint8)address] = data;
    return;
  }

  if(address == 0xff40) {  //LCDC
    if(status.displayEnable != data.bit(7)) {
      status.mode = 0;
      status.ly = 0;
      status.lx = 0;

      //restart cothread to begin new frame
      auto clock = Thread::clock();
      Thread::create(4 * 1024 * 1024, {&PPU::main, this});
      Thread::setClock(clock);
    }

    status.displayEnable = data & 0x80;
    status.windowTilemapSelect = data & 0x40;
    status.windowDisplayEnable = data & 0x20;
    status.bgTiledataSelect = data & 0x10;
    status.bgTilemapSelect = data & 0x08;
    status.obSize = data & 0x04;
    status.obEnable = data & 0x02;
    status.bgEnable = data & 0x01;
    return;
  }

  if(address == 0xff41) {  //STAT
    //hardware bug: writes to STAT on non-CGB hardware force-enable flags for 1 T-cycle.
    //this can generate spurious interrupts if the STAT line is currently low.
    //note: this is not 100% accurate ...
    if(!Model::GameBoyColor()) {
      status.interruptHblank = 1;
      status.interruptVblank = 1;
      status.interruptOAM    = 1;
      status.interruptLYC    = 1;
      stat();
    }

    status.interruptHblank = data.bit(3);
    status.interruptVblank = data.bit(4);
    status.interruptOAM    = data.bit(5);
    status.interruptLYC    = data.bit(6);
    return;
  }

  if(address == 0xff42) {  //SCY
    status.scy = data;
    return;
  }

  if(address == 0xff43) {  //SCX
    status.scx = data;
    return;
  }

  if(address == 0xff44) {  //LY
    status.ly = 0;
    return;
  }

  if(address == 0xff45) {  //LYC
    status.lyc = data;
    return;
  }

  if(address == 0xff46) {  //DMA
    status.dmaActive = true;
    status.dmaClock = 0;
    status.dmaBank = data;
    return;
  }

  if(address == 0xff47) {  //BGP
    bgp[3] = (data >> 6) & 3;
    bgp[2] = (data >> 4) & 3;
    bgp[1] = (data >> 2) & 3;
    bgp[0] = (data >> 0) & 3;
    return;
  }

  if(address == 0xff48) {  //OBP0
    obp[0][3] = (data >> 6) & 3;
    obp[0][2] = (data >> 4) & 3;
    obp[0][1] = (data >> 2) & 3;
    obp[0][0] = (data >> 0) & 3;
    return;
  }

  if(address == 0xff49) {  //OBP1
    obp[1][3] = (data >> 6) & 3;
    obp[1][2] = (data >> 4) & 3;
    obp[1][1] = (data >> 2) & 3;
    obp[1][0] = (data >> 0) & 3;
    return;
  }

  if(address == 0xff4a) {  //WY
    status.wy = data;
    return;
  }

  if(address == 0xff4b) {  //WX
    status.wx = data;
    return;
  }

  if(address == 0xff4f) {  //VBK
    status.vramBank = data & 1;
    return;
  }

  if(address == 0xff68) {  //BGPI
    status.bgpiIncrement = data & 0x80;
    status.bgpi = data & 0x3f;
    return;
  }

  if(address == 0xff69) {  //BGPD
    bgpd[status.bgpi] = data;
    if(status.bgpiIncrement) status.bgpi++;
    return;
  }

  if(address == 0xff6a) {  //OBPI
    status.obpiIncrement = data & 0x80;
    status.obpi = data & 0x3f;
  }

  if(address == 0xff6b) {  //OBPD
    obpd[status.obpi] = data;
    if(status.obpiIncrement) status.obpi++;
  }
}
