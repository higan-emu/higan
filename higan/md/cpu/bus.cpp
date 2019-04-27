/* byte reads to word memory areas discard the unselected byte inside the M68K core.
 * as an optimization, the entire word is returned anyway.
 * byte writes to word memory areas that are addressable as bytes cannot enjoy this optimization.
 */

auto CPU::read(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(!io.romEnable) return tmss[address >> 1];
    if(cartridge.node) return cartridge.read(upper, lower, address, data);
    if(expansion.node) return expansion.read(upper, lower, address, data);
    return data;
  }

  if(address >= 0x400000 && address <= 0x7fffff) {
    if(expansion.node) return expansion.read(upper, lower, address, data);
    return data;
  }

  if(address >= 0xa00000 && address <= 0xa0ffff) {
    if(!apu.granted()) return data;
    address.bit(15) = 0;  //a080000-a0ffff mirrors a00000-a07fff
    //word reads load the even input byte into both output bytes
    auto byte = apu.read(address | !upper);  //upper==0 only on odd byte reads
    return byte << 8 | byte << 0;
  }

  if(address >= 0xa10000 && address <= 0xa10fff) {
    return readIO(upper, lower, address & ~0xff00, data);
  }

  if(address >= 0xa11000 && address <= 0xa11fff) {
    return readIO(upper, lower, address & ~0x00ff, data);
  }

  if(address >= 0xa12000 && address <= 0xa12fff) {
    return expansion.readIO(upper, lower, address, data);
  }

  if(address >= 0xa13000 && address <= 0xa13fff) {
    return cartridge.readIO(upper, lower, address, data);
  }

  if(address >= 0xc00000 && address <= 0xdfffff) {
    if(address.bits( 5, 7)) return data;  //should deadlock the machine
    if(address.bits(16,18)) return data;  //should deadlock the machine
    address.bits(8,15) = 0;  //mirrors
    if(address.bit(4)) return data;  //reading the PSG should deadlock the machine
    return vdp.read(address, data);
  }

  if(address >= 0xe00000 && address <= 0xffffff) {
    return ram[address >> 1];
  }

  return data;
}

auto CPU::write(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  if(address >= 0x000000 && address <= 0x3fffff) {
    if(cartridge.node) return cartridge.write(upper, lower, address, data);
    if(expansion.node) return expansion.write(upper, lower, address, data);
    return;
  }

  if(address >= 0x400000 && address <= 0x7fffff) {
    if(expansion.node) return expansion.write(upper, lower, address, data);
    return;
  }

  if(address >= 0xa00000 && address <= 0xa0ffff) {
    if(!apu.granted()) return;
    address.bit(15) = 0;  //a08000-a0ffff mirrors a00000-a07fff
    //word writes store the upper input byte into the lower output byte
    return apu.write(address | !upper, data.byte(upper));  //upper==0 only on odd byte reads
  }

  if(address >= 0xa10000 && address <= 0xa10fff) {
    return writeIO(upper, lower, address & ~0xff00, data);
  }

  if(address >= 0xa11000 && address <= 0xa11fff) {
    return writeIO(upper, lower, address & ~0x00ff, data);
  }

  if(address >= 0xa12000 && address <= 0xa12fff) {
    return expansion.writeIO(upper, lower, address, data);
  }

  if(address >= 0xa13000 && address <= 0xa13fff) {
    return cartridge.writeIO(upper, lower, address, data);
  }

  if(address >= 0xa14000 && address <= 0xa140ff) {
    return writeIO(upper, lower, address & ~0xff00, data);
  }

  if(address >= 0xa14100 && address <= 0xa141ff) {
    return writeIO(upper, lower, address & ~0x00ff, data);
  }

  if(address >= 0xc00000 && address <= 0xdfffff) {
    if(address.bits( 5, 7)) return;  //should deadlock the machine
    if(address.bits(16,18)) return;  //should deadlock the machine
    address.bits(8,15) = 0;  //mirrors
    if(address.bit(4)) {
      if(!lower) return;  //byte writes to even PSG registers has no effect
      return psg.write(data.byte(0));
    }
    return vdp.write(address, data);
  }

  if(address >= 0xc00010 && address <= 0xc00017) {
    return psg.write(data);
  }

  if(address >= 0xe00000 && address <= 0xffffff) {
    if(upper) ram[address >> 1].byte(1) = data.byte(1);
    if(lower) ram[address >> 1].byte(0) = data.byte(0);
    return;
  }
}

auto CPU::readIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> uint16 {
  switch(address) {
  case 0xa10000:
    data.bit(0) =  io.version;       //0 = Model 1; 1 = Model 2+
    data.bit(5) = !MegaCD();         //0 = expansion unit connected; 1 = no expansion unit connected
    data.bit(6) =  Region::PAL();    //0 = NTSC; 1 = PAL
    data.bit(7) = !Region::NTSCJ();  //0 = domestic (Japan); 1 = export
    break;

  case 0xa10002: data = controllerPort1.readData(); break;
  case 0xa10004: data = controllerPort2.readData(); break;
  case 0xa10006: data = extensionPort.readData(); break;

  case 0xa10008: data = controllerPort1.readControl(); break;
  case 0xa1000a: data = controllerPort2.readControl(); break;
  case 0xa1000c: data = extensionPort.readControl(); break;

  case 0xa11100: return !apu.granted();
  }

  return (uint8)data << 8 | (uint8)data << 0;  //todo: this is not confirmed
}

auto CPU::writeIO(uint1 upper, uint1 lower, uint24 address, uint16 data) -> void {
  if(upper) data.byte(0) = data.byte(1);  //todo: this is not confirmed

  switch(address) {
  case 0xa10002: controllerPort1.writeData(data); break;
  case 0xa10004: controllerPort2.writeData(data); break;
  case 0xa10006: extensionPort.writeData(data); break;

  case 0xa10008: controllerPort1.writeControl(data); break;
  case 0xa1000a: controllerPort2.writeControl(data); break;
  case 0xa1000c: extensionPort.writeControl(data); break;

  case 0xa11100: apu.request(data.bit(0)); break;
  case 0xa11200: apu.enable(data.bit(0)); break;

  case 0xa14000:
    io.vdpEnable[0] = data == 0x5345;
    break;

  case 0xa14002:
    io.vdpEnable[1] = data == 0x4741;
    break;

  case 0xa14100:
    io.romEnable = data.bit(0);
    break;
  }
}
