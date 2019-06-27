auto PPU::vramAddress(uint13 address) const -> uint16 {
  return status.vramBank << 13 | address;
}

auto PPU::readIO(uint16 address, uint8 data) -> uint8 {
  if(address >= 0x8000 && address <= 0x9fff) {
    return vram[vramAddress(address)];
  }

  if(address >= 0xfe00 && address <= 0xfe9f) {
    if(status.dmaActive && status.dmaClock >= 8) return 0xff;
    return oam[(uint8)address];
  }

  if(address == 0xff40) {  //LCDC
    data(0) = status.bgEnable;
    data(1) = status.obEnable;
    data(2) = status.obSize;
    data(3) = status.bgTilemapSelect;
    data(4) = status.bgTiledataSelect;
    data(5) = status.windowDisplayEnable;
    data(6) = status.windowTilemapSelect;
    data(7) = status.displayEnable;
    return data;
  }

  if(address == 0xff41) {  //STAT
    data(0) = status.mode(0);
    data(1) = status.mode(1);
    data(2) = status.ly == status.lyc;
    data(3) = status.interruptHblank;
    data(4) = status.interruptVblank;
    data(5) = status.interruptOAM;
    data(6) = status.interruptLYC;
    data(7) = 1;
    return data;
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
    data(0,1) = bgp[0];
    data(2,3) = bgp[1];
    data(4,5) = bgp[2];
    data(6,7) = bgp[3];
    return data;
  }

  if(address == 0xff48) {  //OBP0
    data(0,1) = obp[0][0];
    data(2,3) = obp[1][1];
    data(4,5) = obp[2][2];
    data(6,7) = obp[3][3];
    return data;
  }

  if(address == 0xff49) {  //OBP1
    data(0,1) = obp[1][0];
    data(2,3) = obp[1][1];
    data(4,5) = obp[1][2];
    data(6,7) = obp[1][3];
    return data;
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
    data(0,5) = status.bgpi;
    data(7)   = status.bgpiIncrement;
    return data;
  }

  if(address == 0xff69) {  //BGPD
    return bgpd[status.bgpi];
  }

  if(address == 0xff6a) {  //OBPI
    data(0,5) = status.obpi;
    data(7)   = status.obpiIncrement;
    return data;
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

    status.bgEnable            = data(0);
    status.obEnable            = data(1);
    status.obSize              = data(2);
    status.bgTilemapSelect     = data(3);
    status.bgTiledataSelect    = data(4);
    status.windowDisplayEnable = data(5);
    status.windowTilemapSelect = data(6);
    status.displayEnable       = data(7);
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

    status.interruptHblank = data(3);
    status.interruptVblank = data(4);
    status.interruptOAM    = data(5);
    status.interruptLYC    = data(6);
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
    status.dmaBank   = data;
    status.dmaActive = 1;
    status.dmaClock  = 0;
    return;
  }

  if(address == 0xff47) {  //BGP
    bgp[0] = data(0,1);
    bgp[1] = data(2,3);
    bgp[2] = data(4,5);
    bgp[3] = data(6,7);
    return;
  }

  if(address == 0xff48) {  //OBP0
    obp[0][0] = data(0,1);
    obp[0][1] = data(2,3);
    obp[0][2] = data(4,5);
    obp[0][3] = data(6,7);
    return;
  }

  if(address == 0xff49) {  //OBP1
    obp[1][0] = data(0,1);
    obp[1][1] = data(2,3);
    obp[1][2] = data(4,5);
    obp[1][3] = data(6,7);
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
    status.vramBank = data.bit(0);
    return;
  }

  if(address == 0xff68) {  //BGPI
    status.bgpi          = data(0,5);
    status.bgpiIncrement = data(7);
    return;
  }

  if(address == 0xff69) {  //BGPD
    bgpd[status.bgpi] = data;
    if(status.bgpiIncrement) status.bgpi++;
    return;
  }

  if(address == 0xff6a) {  //OBPI
    status.obpi          = data(0,5);
    status.obpiIncrement = data(7);
  }

  if(address == 0xff6b) {  //OBPD
    obpd[status.obpi] = data;
    if(status.obpiIncrement) status.obpi++;
  }
}
