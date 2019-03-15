//CS# 16-bit width mode is not emulated, accesses are always treated as 8-bit
//CS3 /CAS mode is irrelevant since nothing is connected to either /CS3 or /CAS

auto CPU::ChipSelect::select() -> void {
  switch(wait) {
  case 0:  //2 states
    cpu.step(2);
    break;
  case 1:  //1 state
    cpu.step(1);
    break;
  case 2:  //1+n states
    cpu.step(1);
    while(cpu.wait) cpu.step(1);
    break;
  case 3:  //0 states
    break;
  }
}

auto CPU::read(uint24 address) -> uint8 {
  mar = address;

  if(cs0.enable && !(uint24)((address ^ cs0.address) & ~cs0.mask)) {
    cs0.select();
    return mdr = cartridge.read(0, address);
  }

  if(cs1.enable && !(uint24)((address ^ cs1.address) & ~cs1.mask)) {
static uint ctr=0;
if(++ctr>30)
tracing=true;
    cs1.select();
    return mdr = cartridge.read(1, address);
  }

  if(cs2.enable) {
    if(!cs2.mode) {
      if(address >= 0x000080 & address <= 0xffffff) {
        cs2.select();
        return mdr = 0x00;
      }
    } else {
      if(!(uint24)((address ^ cs2.address) & ~cs2.mask)) {
        cs2.select();
        return mdr = 0x00;
      }
    }
  }

  if(cs3.enable && !(uint24)((address ^ cs3.address) & ~cs3.mask)) {
    cs3.select();
    return mdr = 0x00;
  }

  csx.select();
  if(address >= 0x000000 && address <= 0x0000ff) return mdr = cpu.readIO(address);
  if(address >= 0x004000 && address <= 0x006fff) return mdr = cpu.ram.read(address);
  if(address >= 0x007000 && address <= 0x007fff) return mdr = apu.ram.read(address);
  if(address >= 0x008000 && address <= 0x00bfff) return mdr = vpu.read(address);
  if(address >= 0xff0000 && address <= 0xffffff) return mdr = system.bios.read(address);
  return mdr = 0x00;
}

auto CPU::write(uint24 address, uint8 data) -> void {
  mar = address;
  mdr = data;

  if(cs0.enable && !(uint24)((address ^ cs0.address) & ~cs0.mask)) {
    cs0.select();
    return cartridge.write(0, address, data);
  }

  if(cs1.enable && !(uint24)((address ^ cs1.address) & ~cs1.mask)) {
    cs1.select();
    return cartridge.write(1, address, data);
  }

  if(cs2.enable) {
    if(!cs2.mode) {
      if(address >= 0x000080 && address <= 0xffffff) {
        cs2.select();
        return;
      }
    } else {
      if(!(uint24)((address ^ cs2.address) & ~cs2.mask)) {
        cs2.select();
        return;
      }
    }
  }

  if(cs3.enable && !(uint24)((address ^ cs3.address) & ~cs3.mask)) {
    cs3.select();
    return;
  }

  csx.select();
  if(address >= 0x000000 && address <= 0x0000ff) return cpu.writeIO(address, data);
  if(address >= 0x004000 && address <= 0x006fff) return cpu.ram.write(address, data);
  if(address >= 0x007000 && address <= 0x007fff) return apu.ram.write(address, data);
  if(address >= 0x008000 && address <= 0x00bfff) return vpu.write(address, data);
  if(address >= 0xff0000 && address <= 0xffffff) return system.bios.write(address, data);
}
