auto VPU::read(uint24 address) -> uint8 {
  address = 0x8000 | (uint14)address;
  if(address >= 0x8800 && address <= 0x88ff) return spriteRAM.read(address);
  if(address >= 0x8900 && address <= 0x8fff) return 0xff;  //todo: does spriteRAM mirror here?
  if(address >= 0x9000 && address <= 0x9fff) return scrollRAM.read(address);
  if(address >= 0xa000 && address <= 0xbfff) return characterRAM.read(address);

  uint8 data = 0xff;

  switch(address) {
  case 0x8000:
    data.bit(6) = io.hblankEnableIRQ;
    data.bit(7) = io.vblankEnableIRQ;
    break;

  case 0x8002: data = io.window.horigin; break;
  case 0x8003: data = io.window.vorigin; break;
  case 0x8004: data = io.window.hsize; break;
  case 0x8005: data = io.window.vsize; break;

  case 0x8008: data = io.hcounter >> 1; break;
  case 0x8009: data = io.vcounter; break;

  case 0x8010:
    data.bit(6) = io.vblankActive;
    data.bit(7) = io.characterOver;
    break;

  case 0x8012:
    data.bits(0,2) = io.outsideWindowColor;
    data.bit(7) = io.negateScreen;
    break;

  case 0x8020: data = io.sprite.hoffset; break;
  case 0x8021: data = io.sprite.voffset; break;

  case 0x8030: data.bit(7) = io.planePriority; break;

  case 0x8032: data = io.plane1.hscroll; break;
  case 0x8033: data = io.plane1.vscroll; break;
  case 0x8034: data = io.plane2.hscroll; break;
  case 0x8035: data = io.plane2.vscroll; break;

  case 0x8100: break;
  case 0x8101: data.bit(0) = io.sprite.palette[0].bit(1); break;
  case 0x8102: data.bit(0) = io.sprite.palette[0].bit(2); break;
  case 0x8103: data.bit(0) = io.sprite.palette[0].bit(3); break;
  case 0x8104: break;
  case 0x8105: data.bit(0) = io.sprite.palette[1].bit(1); break;
  case 0x8106: data.bit(0) = io.sprite.palette[1].bit(2); break;
  case 0x8107: data.bit(0) = io.sprite.palette[1].bit(3); break;

  case 0x8108: break;
  case 0x8109: data.bit(0) = io.plane1.palette[0].bit(1); break;
  case 0x810a: data.bit(0) = io.plane1.palette[0].bit(2); break;
  case 0x810b: data.bit(0) = io.plane1.palette[0].bit(3); break;
  case 0x810c: break;
  case 0x810d: data.bit(0) = io.plane1.palette[1].bit(1); break;
  case 0x810e: data.bit(0) = io.plane1.palette[1].bit(2); break;
  case 0x810f: data.bit(0) = io.plane1.palette[1].bit(3); break;

  case 0x8110: break;
  case 0x8111: data.bit(0) = io.plane2.palette[0].bit(1); break;
  case 0x8112: data.bit(0) = io.plane2.palette[0].bit(2); break;
  case 0x8113: data.bit(0) = io.plane2.palette[0].bit(3); break;
  case 0x8114: break;
  case 0x8115: data.bit(0) = io.plane2.palette[1].bit(1); break;
  case 0x8116: data.bit(0) = io.plane2.palette[1].bit(2); break;
  case 0x8117: data.bit(0) = io.plane2.palette[1].bit(3); break;

  case 0x8400: data = io.led.control; break;
  case 0x8402: data = io.led.frequency; break;

  case 0x87fe: data = 0x3f; break;  //input port register (reserved)
  }

  return data;
}

auto VPU::write(uint24 address, uint8 data) -> void {
  address = 0x8000 | (uint14)address;
  if(address >= 0x8800 && address <= 0x88ff) return spriteRAM.write(address, data);
  if(address >= 0x8900 && address <= 0x8fff) return;  //todo: does spriteRAM mirror here?
  if(address >= 0x9000 && address <= 0x9fff) return scrollRAM.write(address, data);
  if(address >= 0xa000 && address <= 0xbfff) return characterRAM.write(address, data);

  switch(address) {
  case 0x8000:
    io.hblankEnableIRQ = data.bit(6);
    io.vblankEnableIRQ = data.bit(7);
    cpu.setInterruptHblank(io.hblankActive & io.hblankEnableIRQ);
    cpu.setInterruptVblank(io.vblankActive & io.vblankEnableIRQ);
    break;

  case 0x8002: io.window.horigin = data; break;
  case 0x8003: io.window.vorigin = data; break;
  case 0x8004: io.window.hsize = data; break;
  case 0x8005: io.window.vsize = data; break;

  case 0x8012:
    io.outsideWindowColor = data.bits(0,2);
    io.negateScreen = data.bit(7);
    break;

  case 0x8020: io.sprite.hoffset = data; break;
  case 0x8021: io.sprite.voffset = data; break;

  case 0x8030: io.planePriority = data.bit(7); break;

  case 0x8032: io.plane1.hscroll = data; break;
  case 0x8033: io.plane1.vscroll = data; break;
  case 0x8034: io.plane2.hscroll = data; break;
  case 0x8035: io.plane2.vscroll = data; break;

  case 0x8100: break;
  case 0x8101: io.sprite.palette[0].bit(1) = data.bit(0); break;
  case 0x8102: io.sprite.palette[0].bit(2) = data.bit(0); break;
  case 0x8103: io.sprite.palette[0].bit(3) = data.bit(0); break;
  case 0x8104: break;
  case 0x8105: io.sprite.palette[1].bit(1) = data.bit(0); break;
  case 0x8106: io.sprite.palette[1].bit(2) = data.bit(0); break;
  case 0x8107: io.sprite.palette[1].bit(3) = data.bit(0); break;

  case 0x8108: break;
  case 0x8109: io.plane1.palette[0].bit(1) = data.bit(0); break;
  case 0x810a: io.plane1.palette[0].bit(2) = data.bit(0); break;
  case 0x810b: io.plane1.palette[0].bit(3) = data.bit(0); break;
  case 0x810c: break;
  case 0x810d: io.plane1.palette[1].bit(1) = data.bit(0); break;
  case 0x810e: io.plane1.palette[1].bit(2) = data.bit(0); break;
  case 0x810f: io.plane1.palette[1].bit(3) = data.bit(0); break;

  case 0x8110: break;
  case 0x8111: io.plane2.palette[0].bit(1) = data.bit(0); break;
  case 0x8112: io.plane2.palette[0].bit(2) = data.bit(0); break;
  case 0x8113: io.plane2.palette[0].bit(3) = data.bit(0); break;
  case 0x8114: break;
  case 0x8115: io.plane2.palette[1].bit(1) = data.bit(0); break;
  case 0x8116: io.plane2.palette[1].bit(2) = data.bit(0); break;
  case 0x8117: io.plane2.palette[1].bit(3) = data.bit(0); break;

  case 0x8400: io.led.control.bits(3,7) = data.bits(3,7); break;
  case 0x8402: io.led.frequency = data; break;

  case 0x87e0:
    if(data == 0x52) io = {};
    break;
  }
}
