auto VPU::read(uint24 address) -> uint8 {
  address = 0x8000 | (uint14)address;
  if(address >= 0x8200 && address <= 0x83ff && Model::NeoGeoPocketColor()) return readPaletteColor(address);
  if(address >= 0x8800 && address <= 0x88ff) return readSprite(address);
  if(address >= 0x8c00 && address <= 0x8c3f && Model::NeoGeoPocketColor()) return readSpriteColor(address);
  if(address >= 0x9000 && address <= 0x9fff) return scrollRAM.read(address);
  if(address >= 0xa000 && address <= 0xbfff) return characterRAM.read(address);

  uint8 data = 0xff;

  switch(address) {
  case 0x8000:
    data.bit(6) = io.hblankEnableIRQ;
    data.bit(7) = io.vblankEnableIRQ;
    break;

  case 0x8002: data = window.hoffset; break;
  case 0x8003: data = window.voffset; break;
  case 0x8004: data = window.hlength; break;
  case 0x8005: data = window.vlength; break;

  case 0x8006: data = io.vlines; break;

  case 0x8008: data = io.hcounter * 3 >> 2; break;  //hack
  case 0x8009: data = io.vcounter; break;

  case 0x8010:
    data.bit(6) = io.vblankActive;
    data.bit(7) = io.characterOver;
    break;

  case 0x8012:
    data.bits(0,2) = window.color;
    data.bit(7) = screen.negate;
    break;

  case 0x8020: data = sprite.hscroll; break;
  case 0x8021: data = sprite.vscroll; break;

  case 0x8030: data.bit(7) = io.planePriority; break;

  case 0x8032: data = plane1.hscroll; break;
  case 0x8033: data = plane1.vscroll; break;
  case 0x8034: data = plane2.hscroll; break;
  case 0x8035: data = plane2.vscroll; break;

  case 0x8100: data.bits(0,2) = sprite.palette[0][0]; break;
  case 0x8101: data.bits(0,2) = sprite.palette[0][1]; break;
  case 0x8102: data.bits(0,2) = sprite.palette[0][2]; break;
  case 0x8103: data.bits(0,2) = sprite.palette[0][3]; break;
  case 0x8104: data.bits(0,2) = sprite.palette[1][0]; break;
  case 0x8105: data.bits(0,2) = sprite.palette[1][1]; break;
  case 0x8106: data.bits(0,2) = sprite.palette[1][2]; break;
  case 0x8107: data.bits(0,2) = sprite.palette[1][3]; break;

  case 0x8108: data.bits(0,2) = plane1.palette[0][0]; break;
  case 0x8109: data.bits(0,2) = plane1.palette[0][1]; break;
  case 0x810a: data.bits(0,2) = plane1.palette[0][2]; break;
  case 0x810b: data.bits(0,2) = plane1.palette[0][3]; break;
  case 0x810c: data.bits(0,2) = plane1.palette[1][0]; break;
  case 0x810d: data.bits(0,2) = plane1.palette[1][1]; break;
  case 0x810e: data.bits(0,2) = plane1.palette[1][2]; break;
  case 0x810f: data.bits(0,2) = plane1.palette[1][3]; break;

  case 0x8110: data.bits(0,2) = plane2.palette[0][0]; break;
  case 0x8111: data.bits(0,2) = plane2.palette[0][1]; break;
  case 0x8112: data.bits(0,2) = plane2.palette[0][2]; break;
  case 0x8113: data.bits(0,2) = plane2.palette[0][3]; break;
  case 0x8114: data.bits(0,2) = plane2.palette[1][0]; break;
  case 0x8115: data.bits(0,2) = plane2.palette[1][1]; break;
  case 0x8116: data.bits(0,2) = plane2.palette[1][2]; break;
  case 0x8117: data.bits(0,2) = plane2.palette[1][3]; break;

  case 0x8118:
    if(!Model::NeoGeoPocketColor()) break;
    data.bits(0,2) = background.color;
    data.bits(3,5) = background.unused;
    data.bits(6,7) = background.mode;
    break;

  case 0x8400: data = led.control; break;
  case 0x8402: data = led.frequency; break;

  case 0x87e2:
    if(!Model::NeoGeoPocketColor()) break;
    data.bits(0,6) = 0b000000;
    data.bit(7) = screen.colorMode;
    break;

  case 0x87fe: data = 0x3f; break;  //input port register (reserved)
  }

  return data;
}

auto VPU::write(uint24 address, uint8 data) -> void {
  address = 0x8000 | (uint14)address;
  if(address >= 0x8200 && address <= 0x83ff && Model::NeoGeoPocketColor()) return writePaletteColor(address, data);
  if(address >= 0x8800 && address <= 0x88ff) return writeSprite(address, data);
  if(address >= 0x8c00 && address <= 0x8cff && Model::NeoGeoPocketColor()) return writeSpriteColor(address, data);
  if(address >= 0x9000 && address <= 0x9fff) return scrollRAM.write(address, data);
  if(address >= 0xa000 && address <= 0xbfff) return characterRAM.write(address, data);

  switch(address) {
  case 0x8000:
    io.hblankEnableIRQ = data.bit(6);
    io.vblankEnableIRQ = data.bit(7);
  //cpu.setInterruptHblank(io.hblankActive & io.hblankEnableIRQ);
    cpu.int4.set(io.vblankActive & io.vblankEnableIRQ);
    break;

  case 0x8002: window.hoffset = data; break;
  case 0x8003: window.voffset = data; break;
  case 0x8004: window.hlength = data; break;
  case 0x8005: window.vlength = data; break;

  case 0x8006:
    io.vlines = max(152, data);  //it is unknown if this value can be set below the height of the LCD screen
    break;

  case 0x8012:
    window.color = data.bits(0,2);
    screen.negate = data.bit(7);
    break;

  case 0x8020: sprite.hscroll = data; break;
  case 0x8021: sprite.vscroll = data; break;

  case 0x8030: io.planePriority = data.bit(7); break;

  case 0x8032: plane1.hscroll = data; break;
  case 0x8033: plane1.vscroll = data; break;
  case 0x8034: plane2.hscroll = data; break;
  case 0x8035: plane2.vscroll = data; break;

  case 0x8100: break;
  case 0x8101: sprite.palette[0][1] = data.bits(0,2); break;
  case 0x8102: sprite.palette[0][2] = data.bits(0,2); break;
  case 0x8103: sprite.palette[0][3] = data.bits(0,2); break;
  case 0x8104: break;
  case 0x8105: sprite.palette[1][1] = data.bits(0,2); break;
  case 0x8106: sprite.palette[1][2] = data.bits(0,2); break;
  case 0x8107: sprite.palette[1][3] = data.bits(0,2); break;

  case 0x8108: break;
  case 0x8109: plane1.palette[0][1] = data.bits(0,2); break;
  case 0x810a: plane1.palette[0][2] = data.bits(0,2); break;
  case 0x810b: plane1.palette[0][3] = data.bits(0,2); break;
  case 0x810c: break;
  case 0x810d: plane1.palette[1][1] = data.bits(0,2); break;
  case 0x810e: plane1.palette[1][2] = data.bits(0,2); break;
  case 0x810f: plane1.palette[1][3] = data.bits(0,2); break;

  case 0x8110: break;
  case 0x8111: plane2.palette[0][1] = data.bits(0,2); break;
  case 0x8112: plane2.palette[0][2] = data.bits(0,2); break;
  case 0x8113: plane2.palette[0][3] = data.bits(0,2); break;
  case 0x8114: break;
  case 0x8115: plane2.palette[1][1] = data.bits(0,2); break;
  case 0x8116: plane2.palette[1][2] = data.bits(0,2); break;
  case 0x8117: plane2.palette[1][3] = data.bits(0,2); break;

  case 0x8118:
    if(!Model::NeoGeoPocketColor()) break;
    background.color = data.bits(0,2);
    background.unused = data.bits(3,5);
    background.mode = data.bits(6,7);
    break;

  case 0x8400: led.control.bits(3,7) = data.bits(3,7); break;
  case 0x8402: led.frequency = data; break;

  case 0x87e0:
    if(data == 0x52) io = {};
    break;

  case 0x87e2:
    if(!Model::NeoGeoPocketColor()) break;
    screen.colorMode = data.bit(7);
    break;
  }
}

auto VPU::readSprite(uint8 address) -> uint8 {
  auto& s = sprites[address >> 2];
  switch(address & 3) {
  case 0: return s.character;
  case 1: return s.character >> 8 | s.vchain << 1 | s.hchain << 2 | s.priority << 3 | s.palette << 5 | s.vflip << 6 | s.hflip << 7;
  case 2: return s.hoffset;
  case 3: return s.voffset;
  }
  unreachable;
}

auto VPU::writeSprite(uint8 address, uint8 data) -> void {
  auto& s = sprites[address >> 2];
  switch(address & 3) {
  case 0:
    s.character.bits(0,7) = data;
    break;
  case 1:
    s.character.bit(8) = data.bit(0);
    s.vchain = data.bit(1);
    s.hchain = data.bit(2);
    s.priority = data.bits(3,4);
    s.palette = data.bit(5);
    s.vflip = data.bit(6);
    s.hflip = data.bit(7);
    break;
  case 2:
    s.hoffset = data;
    break;
  case 3:
    s.voffset = data;
    break;
  }
}

auto VPU::readSpriteColor(uint6 address) -> uint8 {
  return sprites[address].code;  //d4-d7 = 0
}

auto VPU::writeSpriteColor(uint6 address, uint8 data) -> void {
  sprites[address].code = data.bits(0,3);
}

auto VPU::readPaletteColor(uint9 address) -> uint8 {
  if(!address.bit(0)) {
    return colorPalette[address >> 1].bits(0, 7);
  } else {
    return colorPalette[address >> 1].bits(8,11);  //d4-d7 = 0
  }
}

auto VPU::writePaletteColor(uint9 address, uint8 data) -> void {
  if(!address.bit(0)) {
    colorPalette[address >> 1].bits(0, 7) = data.bits(0,7);
  } else {
    colorPalette[address >> 1].bits(8,11) = data.bits(0,3);
  }
}
