auto VDP::read(uint24 address, uint16) -> uint16 {
  switch(address & 0xc0001e) {

  //data port
  case 0xc00000: case 0xc00002: {
    return readDataPort();
  }

  //control port
  case 0xc00004: case 0xc00006: {
    return readControlPort();
  }

  //counter
  case 0xc00008: case 0xc0000a: case 0xc0000c: case 0xc0000e: {
    auto vcounter = state.vcounter;
    if(io.interlaceMode.bit(0)) {
      if(io.interlaceMode.bit(1)) vcounter <<= 1;
      vcounter.bit(0) = vcounter.bit(8);
    }
    return vcounter << 8 | (state.hdot >> 1) << 0;
  }

  }

  return 0x0000;
}

auto VDP::write(uint24 address, uint16 data) -> void {
  switch(address & 0xc0001e) {

  //data port
  case 0xc00000: case 0xc00002: {
    return writeDataPort(data);
  }

  //control port
  case 0xc00004: case 0xc00006: {
    return writeControlPort(data);
  }

  }
}

//

auto VDP::readDataPort() -> uint16 {
  io.commandPending = false;

  //VRAM read
  if(io.command.bit(0,3) == 0) {
    auto address = io.address.bit(1,16);
    auto data = vram.read(address);
    io.address += io.dataIncrement;
    return data;
  }

  //VSRAM read
  if(io.command.bit(0,3) == 4) {
    auto address = io.address.bit(1,6);
    auto data = vsram.read(address);
    io.address += io.dataIncrement;
    return data;
  }

  //CRAM read
  if(io.command.bit(0,3) == 8) {
    auto address = io.address.bit(1,6);
    auto data = cram.read(address);
    io.address += io.dataIncrement;
    return data.bit(0,2) << 1 | data.bit(3,5) << 5 | data.bit(6,8) << 9;
  }

  return 0x0000;
}

auto VDP::writeDataPort(uint16 data) -> void {
  io.commandPending = false;

  //DMA VRAM fill
  if(dma.io.wait) {
    dma.io.wait = false;
    dma.io.fill = data >> 8;
    //falls through to memory write
    //causes extra transfer to occur on VRAM fill operations
  }

  //VRAM write
  if(io.command.bit(0,3) == 1) {
    auto address = io.address.bit(1,16);
    if(io.address.bit(0)) data = data >> 8 | data << 8;
    vram.write(address, data);
    io.address += io.dataIncrement;
    return;
  }

  //VSRAM write
  if(io.command.bit(0,3) == 5) {
    auto address = io.address.bit(1,6);
    //data format: ---- --yy yyyy yyyy
    vsram.write(address, data.bit(0,9));
    io.address += io.dataIncrement;
    return;
  }

  //CRAM write
  if(io.command.bit(0,3) == 3) {
    auto address = io.address.bit(1,6);
    //data format: ---- bbb- ggg- rrr-
    cram.write(address, data.bit(1,3) << 0 | data.bit(5,7) << 3 | data.bit(9,11) << 6);
    io.address += io.dataIncrement;
    return;
  }
}

//

auto VDP::readControlPort() -> uint16 {
  io.commandPending = false;

  uint16 result;
  result.bit( 0) = Region::PAL();
  result.bit( 1) = io.command.bit(5);  //DMA active
  result.bit( 2) = state.hcounter >= 1280;  //horizontal blank
  result.bit( 3) = state.vcounter >= screenHeight();  //vertical blank
  result.bit( 4) = io.interlaceMode.bit(0) && state.field;
  result.bit( 5) = 0;  //SCOL
  result.bit( 6) = 0;  //SOVR
  result.bit( 7) = io.vblankIRQ;
  result.bit( 8) = 0;  //FIFO full
  result.bit( 9) = 1;  //FIFO empty
  result.bit(10) = 1;  //constants (bits 10-15)
  result.bit(11) = 0;
  result.bit(12) = 1;
  result.bit(13) = 1;
  result.bit(14) = 0;
  result.bit(15) = 0;
  return result;
}

auto VDP::writeControlPort(uint16 data) -> void {
  //command write (lo)
  if(io.commandPending) {
    io.commandPending = false;

    io.command.bit(2,5) = data.bit(4,7);
    io.address.bit(14,16) = data.bit(0,2);

    if(!dma.io.enable) io.command.bit(5) = 0;
    if(dma.io.mode == 3) dma.io.wait = false;
    return;
  }

  //command/address bits are always set here, even for register writes
  io.command.bit(0,1) = data.bit(14,15);
  io.address.bit(0,13) = data.bit(0,13);

  //command write (hi)
  if(data.bit(14,15) != 2) {
    io.commandPending = true;
    return;
  }

  //register write (d13 is ignored)
  switch(data.bit(8,12)) {

  //mode register 1
  case 0x00: {
    io.displayOverlayEnable = data.bit(0);
    io.counterLatch = data.bit(1);
    io.horizontalBlankInterruptEnable = data.bit(4);
    io.leftColumnBlank = data.bit(5);
    return;
  }

  //mode register 2
  case 0x01: {
    io.videoMode = data.bit(2);
    io.overscan = data.bit(3);
    dma.io.enable = data.bit(4);
    io.verticalBlankInterruptEnable = data.bit(5);
    io.displayEnable = data.bit(6);
    vram.mode = data.bit(7);
    if(!dma.io.enable) io.command.bit(5) = 0;
    return;
  }

  //plane A name table location
  case 0x02: {
    planeA.io.nametableAddress.bit(12,15) = data.bit(3,6);
    return;
  }

  //window name table location
  case 0x03: {
    window.io.nametableAddress.bit(10,15) = data.bit(1,6);
    return;
  }

  //plane B name table location
  case 0x04: {
    planeB.io.nametableAddress.bit(12,15) = data.bit(0,3);
    return;
  }

  //sprite attribute table location
  case 0x05: {
    sprite.io.nametableAddress.bit(8,15) = data.bit(0,7);
    return;
  }

  //sprite pattern base address
  case 0x06: {
    sprite.io.generatorAddress.bit(15) = data.bit(5);
    return;
  }

  //background color
  case 0x07: {
    io.backgroundColor = data.bit(0,5);
    return;
  }

  //horizontal interrupt counter
  case 0x0a: {
    io.horizontalInterruptCounter = data.bit(0,7);
    return;
  }

  //mode register 3
  case 0x0b: {
    planeA.io.horizontalScrollMode = data.bit(0,1);
    planeB.io.horizontalScrollMode = data.bit(0,1);
    planeA.io.verticalScrollMode = data.bit(2);
    planeB.io.verticalScrollMode = data.bit(2);
    io.externalInterruptEnable = data.bit(3);
    return;
  }

  //mode register 4
  case 0x0c: {
    io.displayWidth = data.bit(0) | data.bit(7) << 1;
    io.interlaceMode = data.bit(1,2);
    io.shadowHighlightEnable = data.bit(3);
    io.externalColorEnable = data.bit(4);
    io.horizontalSync = data.bit(5);
    io.verticalSync = data.bit(6);
    return;
  }

  //horizontal scroll data location
  case 0x0d: {
    planeA.io.horizontalScrollAddress = data.bit(0,6) << 9;
    planeB.io.horizontalScrollAddress = data.bit(0,6) << 9;
    return;
  }

  //nametable pattern base address
  case 0x0e: {
    //bit(0) relocates plane A to the extended VRAM region.
    //bit(4) relocates plane B, but only when bit(0) is also set.
    planeA.io.generatorAddress.bit(15) = data.bit(0);
    planeB.io.generatorAddress.bit(15) = data.bit(4) && data.bit(0);
    return;
  }

  //data port auto-increment value
  case 0x0f: {
    io.dataIncrement = data.bit(0,7);
    return;
  }

  //plane size
  case 0x10: {
    planeA.io.nametableWidth = data.bit(0,1);
    planeB.io.nametableWidth = data.bit(0,1);
    planeA.io.nametableHeight = data.bit(4,5);
    planeB.io.nametableHeight = data.bit(4,5);
    return;
  }

  //window plane horizontal position
  case 0x11: {
    window.io.horizontalOffset = data.bit(0,4) << 4;
    window.io.horizontalDirection = data.bit(7);
    return;
  }

  //window plane vertical position
  case 0x12: {
    window.io.verticalOffset = data.bit(0,4) << 3;
    window.io.verticalDirection = data.bit(7);
    return;
  }

  //DMA length
  case 0x13: {
    dma.io.length.bit(0,7) = data.bit(0,7);
    return;
  }

  //DMA length
  case 0x14: {
    dma.io.length.bit(8,15) = data.bit(0,7);
    return;
  }

  //DMA source
  case 0x15: {
    dma.io.source.bit(0,7) = data.bit(0,7);
    return;
  }

  //DMA source
  case 0x16: {
    dma.io.source.bit(8,15) = data.bit(0,7);
    return;
  }

  //DMA source
  case 0x17: {
    dma.io.source.bit(16,21) = data.bit(0,5);
    dma.io.mode = data.bit(6,7);
    dma.io.wait = dma.io.mode.bit(1);
    return;
  }

  //unused
  default: {
    return;
  }

  }
}
