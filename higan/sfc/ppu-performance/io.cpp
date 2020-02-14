auto PPU::latchCounters() -> void {
  cpu.synchronize(ppu);
  io.hcounter = cpu.hdot();
  io.vcounter = cpu.vcounter();
  latch.counters = 1;
}

auto PPU::addressVRAM() const -> uint16 {
  uint16 address = io.vramAddress;
  switch(io.vramMapping) {
  case 0: return address;
  case 1: return address.bit( 8,15) <<  8 | address.bit(0,4) << 3 | address.bit(5,7);
  case 2: return address.bit( 9,15) <<  9 | address.bit(0,5) << 3 | address.bit(6,8);
  case 3: return address.bit(10,15) << 10 | address.bit(0,6) << 3 | address.bit(7,9);
  }
  unreachable;
}

auto PPU::readVRAM() -> uint16 {
  if(!io.displayDisable && vcounter() < vdisp()) return 0x0000;
  auto address = addressVRAM();
  return vram[address];
}

auto PPU::writeVRAM(uint1 byte, uint8 data) -> void {
  if(!io.displayDisable && vcounter() < vdisp()) return;
  auto address = addressVRAM();
  vram[address].byte(byte) = data;
}

auto PPU::readOAM(uint10 address) -> uint8 {
  if(!io.displayDisable && vcounter() < vdisp()) {
    if(address.bit(9) == 0) return obj.oam.read(0x000 | latch.oamAddress << 2 | address & 1);
    if(address.bit(9) == 1) return obj.oam.read(0x200 | latch.oamAddress >> 2);
  }
  return obj.oam.read(address);
}

auto PPU::writeOAM(uint10 address, uint8 data) -> void {
  if(!io.displayDisable && vcounter() < vdisp()) {
    if(address.bit(9) == 0) return obj.oam.write(0x000 | latch.oamAddress << 2 | address & 1, data);
    if(address.bit(9) == 1) return obj.oam.write(0x200 | latch.oamAddress >> 2, data);
  }
  return obj.oam.write(address, data);
}

auto PPU::readCGRAM(uint1 byte, uint8 address) -> uint8 {
  if(!io.displayDisable
  && vcounter() > 0 && vcounter() < vdisp()
  && hcounter() >= 88 && hcounter() < 1096
  ) address = latch.cgramAddress;
  return dac.cgram[address].byte(byte);
}

auto PPU::writeCGRAM(uint8 address, uint15 data) -> void {
  if(!io.displayDisable
  && vcounter() > 0 && vcounter() < vdisp()
  && hcounter() >= 88 && hcounter() < 1096
  ) address = latch.cgramAddress;
  dac.cgram[address] = data;
}

auto PPU::readIO(uint24 address, uint8 data) -> uint8 {
  cpu.synchronize(ppu);

  switch((uint16)address) {

  case 0x2104: case 0x2105: case 0x2106: case 0x2108:
  case 0x2109: case 0x210a: case 0x2114: case 0x2115:
  case 0x2116: case 0x2118: case 0x2119: case 0x211a:
  case 0x2124: case 0x2125: case 0x2126: case 0x2128:
  case 0x2129: case 0x212a: {
    return ppu1.mdr;
  }

  //MPYL
  case 0x2134: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(0);
  }

  //MPYM
  case 0x2135: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(1);
  }

  //MYPH
  case 0x2136: {
    uint24 result = (int16)io.m7a * (int8)(io.m7b >> 8);
    return ppu1.mdr = result.byte(2);
  }

  //SLHV
  case 0x2137: {
    if(cpu.pio().bit(7)) latchCounters();
    return data;  //CPU MDR
  }

  //OAMDATAREAD
  case 0x2138: {
    ppu1.mdr = readOAM(io.oamAddress++);
    obj.setFirstSprite();
    return ppu1.mdr;
  }

  //VMDATALREAD
  case 0x2139: {
    ppu1.mdr = latch.vram.byte(0);
    if(io.vramIncrementMode == 0) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //VMDATAHREAD
  case 0x213a: {
    ppu1.mdr = latch.vram.byte(1);
    if(io.vramIncrementMode == 1) {
      latch.vram = readVRAM();
      io.vramAddress += io.vramIncrementSize;
    }
    return ppu1.mdr;
  }

  //CGDATAREAD
  case 0x213b: {
    if(io.cgramAddressLatch++ == 0) {
      ppu2.mdr.bit(0,7) = readCGRAM(0, io.cgramAddress);
    } else {
      ppu2.mdr.bit(0,6) = readCGRAM(1, io.cgramAddress++);
    }
    return ppu2.mdr;
  }

  //OPHCT
  case 0x213c: {
    if(latch.hcounter++ == 0) {
      ppu2.mdr.bit(0,7) = io.hcounter.bit(0,7);
    } else {
      ppu2.mdr.bit(0)   = io.hcounter.bit(8);
    }
    return ppu2.mdr;
  }

  //OPVCT
  case 0x213d: {
    if(latch.vcounter++ == 0) {
      ppu2.mdr.bit(0,7) = io.vcounter.bit(0,7);
    } else {
      ppu2.mdr.bit(0)   = io.vcounter.bit(8);
    }
    return ppu2.mdr;
  }

  //STAT77
  case 0x213e: {
    ppu1.mdr.bit(0,3) = ppu1.version;
    ppu1.mdr.bit(5)   = 0;
    ppu1.mdr.bit(6)   = obj.io.rangeOver;
    ppu1.mdr.bit(7)   = obj.io.timeOver;
    return ppu1.mdr;
  }

  //STAT78
  case 0x213f: {
    latch.hcounter = 0;
    latch.vcounter = 0;
    ppu2.mdr.bit(0,3) = ppu2.version;
    ppu2.mdr.bit(4)   = Region::PAL();  //0 = NTSC, 1 = PAL
    if(!cpu.pio().bit(7)) {
      ppu2.mdr.bit(6) = 1;
    } else {
      ppu2.mdr.bit(6) = latch.counters;
      latch.counters = 0;
    }
    ppu2.mdr.bit(7) = cpu.field();
    return ppu2.mdr;
  }

  }

  return data;
}

auto PPU::writeIO(uint24 address, uint8 data) -> void {
  cpu.synchronize(ppu);

  switch((uint16)address) {

  //INIDISP
  case 0x2100: {
    if(io.displayDisable && vcounter() == vdisp()) obj.addressReset();
    io.displayBrightness = data.bit(0,3);
    io.displayDisable    = data.bit(7);
    return;
  }

  //OBSEL
  case 0x2101: {
    obj.io.tiledataAddress = data.bit(0,2) << 13;
    obj.io.nameselect      = data.bit(3,4);
    obj.io.baseSize        = data.bit(5,7);
    return;
  }

  //OAMADDL
  case 0x2102: {
    io.oamBaseAddress.bit(1,8) = data.bit(0,7);
    obj.addressReset();
    return;
  }

  //OAMADDH
  case 0x2103: {
    io.oamBaseAddress.bit(9) = data.bit(0);
    io.oamPriority = data.bit(7);
    obj.addressReset();
    return;
  }

  //OAMDATA
  case 0x2104: {
    uint1 latchBit = io.oamAddress.bit(0);
    uint10 address = io.oamAddress++;
    if(latchBit == 0) latch.oam = data;
    if(address.bit(9)) {
      writeOAM(address, data);
    } else if(latchBit == 1) {
      writeOAM((address & ~1) + 0, latch.oam);
      writeOAM((address & ~1) + 1, data);
    }
    obj.setFirstSprite();
    return;
  }

  //BGMODE
  case 0x2105: {
    io.bgMode       = data.bit(0,2);
    io.bgPriority   = data.bit(3);
    bg1.io.tileSize = data.bit(4);
    bg2.io.tileSize = data.bit(5);
    bg3.io.tileSize = data.bit(6);
    bg4.io.tileSize = data.bit(7);
    updateVideoMode();
    return;
  }

  //MOSAIC
  case 0x2106: {
    //TODO
    return;
  }

  //BG1SC
  case 0x2107: {
    bg1.io.screenSize    = data.bit(0,1);
    bg1.io.screenAddress = data.bit(2,7) << 10;
    return;
  }

  //BG2SC
  case 0x2108: {
    bg2.io.screenSize    = data.bit(0,1);
    bg2.io.screenAddress = data.bit(2,7) << 10;
    return;
  }

  //BG3SC
  case 0x2109: {
    bg3.io.screenSize    = data.bit(0,1);
    bg3.io.screenAddress = data.bit(2,7) << 10;
    return;
  }

  //BG4SC
  case 0x210a: {
    bg4.io.screenSize    = data.bit(0,1);
    bg4.io.screenAddress = data.bit(2,7) << 10;
    return;
  }

  //BG12NBA
  case 0x210b: {
    bg1.io.tiledataAddress = data.bit(0,3) << 12;
    bg2.io.tiledataAddress = data.bit(4,7) << 12;
    return;
  }

  //BG34NBA
  case 0x210c: {
    bg3.io.tiledataAddress = data.bit(0,3) << 12;
    bg4.io.tiledataAddress = data.bit(4,7) << 12;
    return;
  }

  //BG1HOFS
  case 0x210d: {
    io.hoffsetMode7 = data << 8 | latch.mode7;
    latch.mode7 = data;

    bg1.io.hoffset = data << 8 | latch.bgofsPPU1 & ~7 | latch.bgofsPPU2 & 7;
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG1VOFS
  case 0x210e: {
    io.voffsetMode7 = data << 8 | latch.mode7;
    latch.mode7 = data;

    bg1.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG2HOFS
  case 0x210f: {
    bg2.io.hoffset = data << 8 | latch.bgofsPPU1 & ~7 | latch.bgofsPPU2 & 7;
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG2VOFS
  case 0x2110: {
    bg2.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG3HOFS
  case 0x2111: {
    bg3.io.hoffset = data << 8 | latch.bgofsPPU1 & ~7 | latch.bgofsPPU2 & 7;
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG3VOFS
  case 0x2112: {
    bg3.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //BG4HOFS
  case 0x2113: {
    bg4.io.hoffset = data << 8 | latch.bgofsPPU1 & ~7 | latch.bgofsPPU2 & 7;
    latch.bgofsPPU1 = data;
    latch.bgofsPPU2 = data;
    return;
  }

  //BG4VOFS
  case 0x2114: {
    bg4.io.voffset = data << 8 | latch.bgofsPPU1;
    latch.bgofsPPU1 = data;
    return;
  }

  //VMAIN
  case 0x2115: {
    static const uint size[4] = {1, 32, 128, 128};
    io.vramIncrementSize = size[data.bit(0,1)];
    io.vramMapping       = data.bit(2,3);
    io.vramIncrementMode = data.bit(7);
    return;
  }

  //VMADDL
  case 0x2116: {
    io.vramAddress.bit(0,7) = data;
    latch.vram = readVRAM();
    return;
  }

  //VHADDH
  case 0x2117: {
    io.vramAddress.bit(8,15) = data;
    latch.vram = readVRAM();
    return;
  }

  //VMDATAL
  case 0x2118: {
    writeVRAM(0, data);
    if(io.vramIncrementMode == 0) io.vramAddress += io.vramIncrementSize;
    return;
  }

  //VMDATAH
  case 0x2119: {
    writeVRAM(1, data);
    if(io.vramIncrementMode == 1) io.vramAddress += io.vramIncrementSize;
    return;
  }

  //M7SEL
  case 0x211a: {
    io.hflipMode7  = data.bit(0);
    io.vflipMode7  = data.bit(1);
    io.repeatMode7 = data.bit(6,7);
    return;
  }

  //M7A
  case 0x211b: {
    io.m7a = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7B
  case 0x211c: {
    io.m7b = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7C
  case 0x211d: {
    io.m7c = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7D
  case 0x211e: {
    io.m7d = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7X
  case 0x211f: {
    io.m7x = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //M7Y
  case 0x2120: {
    io.m7y = data << 8 | latch.mode7;
    latch.mode7 = data;
    return;
  }

  //CGADD
  case 0x2121: {
    io.cgramAddress = data;
    io.cgramAddressLatch = 0;
    return;
  }

  //CGDATA
  case 0x2122: {
    if(io.cgramAddressLatch++ == 0) {
      latch.cgram = data;
    } else {
      writeCGRAM(io.cgramAddress++, data.bit(0,6) << 8 | latch.cgram);
    }
    return;
  }

  }
}

auto PPU::updateVideoMode() -> void {
}
